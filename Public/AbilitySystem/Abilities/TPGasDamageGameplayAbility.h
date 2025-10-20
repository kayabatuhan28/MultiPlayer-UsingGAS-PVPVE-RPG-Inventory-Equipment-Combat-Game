// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPGasGameplayAbility.h"
#include "TPGasDamageGameplayAbility.generated.h"

UENUM()
enum EAbilityDamageType
{
	DefaultType,
	Magic,
	Physical
};

UCLASS()
class TPGAS_API UTPGasDamageGameplayAbility : public UTPGasGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	float GetCalculatedActualDamage();

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TEnumAsByte<EAbilityDamageType> AbilityDamageType = EAbilityDamageType::Physical;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	FGameplayTag DamagedActorActivateAbilityTag{};

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Data")
	FString SkillName{};

	UPROPERTY(EditDefaultsOnly, Category = "Skill Data")
	FString SkillDescription{};

	
	

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float DamageScale = 1.f;

	
};


