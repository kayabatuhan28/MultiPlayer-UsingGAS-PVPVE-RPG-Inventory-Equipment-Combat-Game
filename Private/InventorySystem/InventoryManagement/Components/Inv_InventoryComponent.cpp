// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Character/TPGasCharacter.h"
#include "Combat/Component/TPGasCombatComp.h"
#include "GameFramework/TPGasPlayerState.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_WeaponBase.h"
#include "InventorySystem/Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "InventorySystem/Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "InventorySystem/Items/Inv_InventoryItem.h"
#include "InventorySystem/Items/Fragments/Inv_ItemFragment.h"


class ATPGasPlayerState;

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
	
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}

	
	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Add stacks to an item that already exists in the inventory. We only want to update to stack count,
		// not create a new item of this type.
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		// This itemType doesn't exist in the inventory. Create a new one and update all pertinent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0, Result.Remainder);
	}
	
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}
	
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UInv_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item); // actually removing the item from our inventory
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(Item, StackCount);
}

void UInv_InventoryComponent::SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount)
{
	// Spawn the dropped item in the level.
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	// Have the item manifest spawn the pickup actor.
	FInv_ItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UInv_InventoryComponent::Server_ConsumeItem_Implementation(UInv_InventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount()-1;
	if (NewStackCount)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	// Get The Consumable Fragment and call Consume()  (Actually create the Consumable Fragment)	
	if (FInv_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
}

void UInv_InventoryComponent::Server_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnEquip)
{
	Multicast_EquipSlotClicked(ItemToEquip, ItemToUnEquip);
}

void UInv_InventoryComponent::Multicast_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnEquip)
{
	// Equipment Component will listen to these delegates
	OnItemEquipped.Broadcast(ItemToEquip);
	OnItemUnEquipped.Broadcast(ItemToUnEquip);
	
	if (IsValid(ItemToUnEquip) && !IsValid(ItemToEquip))
	{
		FInv_ItemManifest& ItemManifest = ItemToUnEquip->GetItemManifestMutable();
		
		if (ItemManifest.GetItemType().MatchesTag(FGameplayTag::RequestGameplayTag("GameItems.Equipment.Weapons")) == false)
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Orange,
			//FString::Printf(TEXT("UnEquipped item silah degil item tag : %s"), *ItemManifest.GetItemType().ToString()));
			return;
		}
		
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Item Not Swap, Just UnEquipped"));
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
		if (!IsValid(PlayerController)) return;

		UTPGasCombatComp* CombatComp = PlayerController->GetPawn()->FindComponentByClass<UTPGasCombatComp>();
		if (!IsValid(CombatComp)) return;

		CombatComp->Server_SetLookingDirection(false);

		ATPGasCharacter* OwnerPlayerCharacter = Cast<ATPGasCharacter>(PlayerController->GetPawn());
		if (!IsValid(PlayerController)) return;

		OwnerPlayerCharacter->SetMovementSpeedReplicated(600.f);

		ATPGasPlayerState* PlayerState = PlayerController->GetPlayerState<ATPGasPlayerState>();
		if (PlayerState == nullptr) return;
		PlayerState->Server_WeaponChangeBroadcast(true);
	}
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
	OnInventoryMenuToggled.Broadcast(bInventoryMenuOpen);
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a PlayerController as Owner."))
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
	
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
	
}













