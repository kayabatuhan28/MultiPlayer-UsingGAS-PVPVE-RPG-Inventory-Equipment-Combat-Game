// Fill out your copyright notice in the Description page of Project Settings.
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor.h"


AInv_EquipActor::AInv_EquipActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AInv_EquipActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SetAttachmentData,0.05f,false);
	
}

void AInv_EquipActor::SetAttachmentData()
{
	
}













