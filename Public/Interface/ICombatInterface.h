// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UICombatInterface : public UInterface
{
	GENERATED_BODY()
};


class TPGAS_API IICombatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	virtual void Die(const FVector& DeathImpulse) = 0;

	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	virtual FOnDamageSignature& GetOnDamageSignature() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnASCRegistered& GetOnAscRegisteredDelegate() = 0;
	
};
