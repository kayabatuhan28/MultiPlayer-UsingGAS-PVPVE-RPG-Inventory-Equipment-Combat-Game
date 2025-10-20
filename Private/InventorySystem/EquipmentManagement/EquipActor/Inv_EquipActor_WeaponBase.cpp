// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_WeaponBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AInv_EquipActor_WeaponBase::AInv_EquipActor_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MainRootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	MainRootComponent->SetupAttachment(GetRootComponent());
	

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(MainRootComponent);
	WeaponMesh->SetCollisionProfileName("NoCollision");
}

void AInv_EquipActor_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInv_EquipActor_WeaponBase::SetAttachmentData()
{
	Super::SetAttachmentData();

	if (!IsValid(GetOwner())) return;

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!IsValid(PC)) return;
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
	if (!IsValid(OwnerASC)) return;

	FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
	OwnerASC->BP_ApplyGameplayEffectToSelf(GE_WeaponEquipped, 1, ContextHandle);
	
}

void AInv_EquipActor_WeaponBase::Destroyed()
{
	Super::Destroyed();

	if (!IsValid(GetOwner())) return;

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!IsValid(PC)) return;
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
	if (!IsValid(OwnerASC)) return;
	
	FGameplayTagContainer ContainerTags{};
	ContainerTags.AddTag(RemovedEquippedGameplayTag);
	OwnerASC->RemoveActiveEffectsWithGrantedTags(ContainerTags);
}



