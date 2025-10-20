// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPGasDamageGameplayAbility.h"
#include "TPGasSummonGameplayAbility.generated.h"


UCLASS()
class TPGAS_API UTPGasSummonGameplayAbility : public UTPGasDamageGameplayAbility
{
	GENERATED_BODY()

public:
 
	UFUNCTION(BlueprintCallable)
	void SpawnSummonActor(TSubclassOf<AActor> SummonedActor, FVector SpawnLocation, FRotator SpawnRotation);

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
