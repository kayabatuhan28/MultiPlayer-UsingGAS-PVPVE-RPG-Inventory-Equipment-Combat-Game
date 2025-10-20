// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_ArmorBase.h"

#include "Character/TPGasCharacter.h"


// Sets default values
AInv_EquipActor_ArmorBase::AInv_EquipActor_ArmorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInv_EquipActor_ArmorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInv_EquipActor_ArmorBase::SetAttachmentData()
{
	Super::SetAttachmentData();

	if (!IsValid(GetOwner())) return;

	if (bSetLeaderPoseData == true)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (!IsValid(PC)) return;
		
		ATPGasCharacter* OwnerPlayer = Cast<ATPGasCharacter>(PC->GetPawn());
		if (IsValid(OwnerPlayer))
		{
			OwnerPlayer->Server_SetBodySkeletalMesh(NewSkeletalMesh, NewSkinnedAsset, EquipActorType);
		}
	}
}

void AInv_EquipActor_ArmorBase::Destroyed()
{
	Super::Destroyed();

	if (!IsValid(GetOwner())) return;

	if (bSetLeaderPoseData == true)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (!IsValid(PC)) return;
		
		ATPGasCharacter* OwnerPlayer = Cast<ATPGasCharacter>(PC->GetPawn());
		if (IsValid(OwnerPlayer))
		{
			OwnerPlayer->Server_SetBodySkeletalMesh(nullptr, nullptr, EquipActorType);
		}
	}
}




