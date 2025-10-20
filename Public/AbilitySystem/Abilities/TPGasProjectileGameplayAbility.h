// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPGasDamageGameplayAbility.h"
#include "TPGasProjectileGameplayAbility.generated.h"

class ATPGasBaseProjectile;

UCLASS()
class TPGAS_API UTPGasProjectileGameplayAbility : public UTPGasDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	// GameplayAbility kütüphanesinden geliyor
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATPGasBaseProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	FVector GetAimImpactPoint();

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
	
};
