// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_ChestActor.generated.h"

UCLASS()
class TPGAS_API AInv_ChestActor : public AActor
{
	GENERATED_BODY()

public:
	AInv_ChestActor();

	UFUNCTION(BlueprintImplementableEvent)
	void ChestOpened(AActor* InteractActor);

	UPROPERTY(BlueprintReadWrite)
	bool bIsChestOpened = false;

protected:
	virtual void BeginPlay() override;


};
