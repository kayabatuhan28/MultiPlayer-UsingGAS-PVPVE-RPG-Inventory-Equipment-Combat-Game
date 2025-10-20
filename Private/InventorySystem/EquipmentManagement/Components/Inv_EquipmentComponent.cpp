// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/EquipmentManagement/Components/Inv_EquipmentComponent.h"

#include "Character/TPGasCharacter.h"
#include "Combat/Component/TPGasCombatComp.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/TPGasPlayerState.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_WeaponBase.h"
#include "InventorySystem/InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventorySystem/InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventorySystem/Items/Inv_InventoryItem.h"
#include "InventorySystem/Items/Fragments/Inv_ItemFragment.h"


void UInv_EquipmentComponent::SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh)
{
	OwningSkeletalMesh = OwningMesh;
}

void UInv_EquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningPlayerController = PlayerController;
	}
	InitInventoryComponent();
}

void UInv_EquipmentComponent::ClearEquipWeaponData(const FGameplayTag& EquipmentTypeTag)
{
	// Weapon UnEquip
	AInv_EquipActor* EquippedActor = FindEquippedActor(EquipmentTypeTag);
	if (!IsValid(EquippedActor)) return;

	
	if (EquippedActor->GetEquipmentType().MatchesTag(FGameplayTag::RequestGameplayTag("GameItems.Equipment.Weapons")))
	{
		if (APawn* Pawn = OwningPlayerController->GetPawn())
		{
			if (ATPGasCharacter* Player = Cast<ATPGasCharacter>(Pawn); IsValid(Player->CombatComp->EquippedWeapon))
			{
				Player->CombatComp->SetNewWeapon(nullptr);
				Player->CombatComp->Server_SetLookingDirection(false);
			}
		}
	}
}

void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerController();
}

void UInv_EquipmentComponent::InitPlayerController()
{
	if (OwningPlayerController = Cast<APlayerController>(GetOwner()); OwningPlayerController.IsValid())
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn());
		if (IsValid(OwnerCharacter))
		{
			OnPossessedPawnChange(nullptr, OwnerCharacter);
		}
		else 
		{
			// Else casesini play as client(dedicated) equip yaptigi zaman beginplayde owningSkeletalMesh valid gelmedigi haliyle 
			// equip yapamadigi icin ekledik. Client icin pawn setlenmesi yetismemekte o yüzden playercontrollerin delegatesini kullandik.
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChange);
		}
	}
}

void UInv_EquipmentComponent::OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn());
	if (IsValid(OwnerCharacter))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}
	InitInventoryComponent();
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid()) return;

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
	}

	if (!InventoryComponent->OnItemUnEquipped.IsAlreadyBound(this, &ThisClass::OnItemUnEquipped))
	{
		InventoryComponent->OnItemUnEquipped.AddDynamic(this, &ThisClass::OnItemUnEquipped);
	}
}

AInv_EquipActor* UInv_EquipmentComponent::SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment, const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	AInv_EquipActor* SpawnedEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquipActor->SetOwner(GetOwner()); // player controller is owner this actor
	EquipmentFragment->SetEquippedActor(SpawnedEquipActor,OwningPlayerController.Get());
	return SpawnedEquipActor;
}

AInv_EquipActor* UInv_EquipmentComponent::FindEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	auto FoundActor = EquippedActors.FindByPredicate([&EquipmentTypeTag](const AInv_EquipActor* EquippedActor)
	{
		return EquippedActor->GetEquipmentType().MatchesTagExact(EquipmentTypeTag);
	});
	return FoundActor ? *FoundActor : nullptr;
}

void UInv_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	AInv_EquipActor* EquippedActor = FindEquippedActor(EquipmentTypeTag);
	if (IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if (!bIsProxy)
	{
		EquipmentFragment->OnEquip(OwningPlayerController.Get());
	}
	
	if (!OwningSkeletalMesh.IsValid()) return;
	AInv_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());

	EquippedActors.Add(SpawnedEquipActor);

	// Weapon Equip
	if (!SpawnedEquipActor->GetEquipmentType().MatchesTag(FGameplayTag::RequestGameplayTag("GameItems.Equipment.Weapons")))
	{
		return;
	}
	
	if (!IsValid(OwningPlayerController.Get()))
	{
		return;
	}

	APawn* Pawn = OwningPlayerController->GetPawn();
	if (!IsValid(Pawn))
	{
		return;
	}

	ATPGasCharacter* Player = Cast<ATPGasCharacter>(Pawn);
	if (!IsValid(Player))
	{
		return;
	}
	
	AInv_EquipActor_WeaponBase* WeaponBase = Cast<AInv_EquipActor_WeaponBase>(SpawnedEquipActor);
	if (Player->CombatComp && IsValid(WeaponBase))
	{
		if (IsValid(Player->CombatComp->EquippedWeapon))
		{
			ClearEquipWeaponData(EquipmentFragment->GetEquipmentType());
		}
		Player->CombatComp->SetNewWeapon(WeaponBase);
		Player->CombatComp->Server_SetLookingDirection(true);
		Player->SetMovementSpeedReplicated(WeaponEquippedMovementSpeed);

		ATPGasPlayerState* PlayerState = OwningPlayerController->GetPlayerState<ATPGasPlayerState>();
		if (IsValid(PlayerState))
		{
			PlayerState->Server_WeaponChangeBroadcast(false);
		}
		
	}
}

void UInv_EquipmentComponent::OnItemUnEquipped(UInv_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FInv_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if (!bIsProxy)
	{
		EquipmentFragment->OnUnEquip(OwningPlayerController.Get());
	}
	
	RemoveEquippedActor(EquipmentFragment->GetEquipmentType());
}





