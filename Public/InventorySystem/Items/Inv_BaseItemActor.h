// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Inv_BaseItemActor.generated.h"

UCLASS()
class TPGAS_API AInv_BaseItemActor : public AActor
{
	GENERATED_BODY()

public:
	
	AInv_BaseItemActor();

	UFUNCTION(BlueprintCallable)
	void DestroyActorWithDelay(float InDelay);

protected:
	
	virtual void BeginPlay() override;


};
