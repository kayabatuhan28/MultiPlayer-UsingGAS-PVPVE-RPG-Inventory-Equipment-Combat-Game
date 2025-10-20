// Fill out your copyright notice in the Description page of Project Settings.
#include "InventorySystem/Items/Inv_BaseItemActor.h"


AInv_BaseItemActor::AInv_BaseItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInv_BaseItemActor::DestroyActorWithDelay(float InDelay)
{
	FTimerHandle TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		Destroy();
	}), InDelay, false);
}

void AInv_BaseItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}



