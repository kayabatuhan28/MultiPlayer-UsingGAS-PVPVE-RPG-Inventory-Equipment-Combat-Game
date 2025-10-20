// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Combat/CombatUtils.h"
#include "TPGasGameplayAbility.generated.h"


UCLASS()
class TPGAS_API UTPGasGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable)
	EInputDirection CalculateInputDirection();

	UFUNCTION(BlueprintCallable)
	void StartStopStaminaRegen(bool bIsStart);

	UFUNCTION(BlueprintCallable)
	void StartStopManaRegen(bool bIsStart);

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:

	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

	
	
};
