// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPGasAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class ATPGasHUD;
class UCharacterClassInfo;
class UGameplayEffect;
class ATPGasBaseCharacter;
class UAbilityInfo;


UCLASS()
class TPGAS_API UTPGasAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "TPGasAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ATPGasHUD*& OutTPGasHUD);

	UFUNCTION(BlueprintPure, Category = "TPGasAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "TPGasAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "TPGasAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "TPGasAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "TPGasAbilitySystemLibrary|Combat")
	static void HealTarget(AActor* HealedActor, TSubclassOf<UGameplayEffect> HealEffectClass, float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "TPGasAbilitySystemLibrary|Combat")
	static void LifeSteal(ATPGasBaseCharacter* BaseCharacter, TSubclassOf<UGameplayEffect> HealEffectClass, float InComingDamage);

	UFUNCTION(BlueprintCallable, Category="TPGasAbilitySystemLibrary|Combat")
	static EHitDirection CalculateHitDirection(AActor* DamageCauser, AActor* DamagedActor);

	UFUNCTION(BlueprintCallable, Category="TPGasAbilitySystemLibrary|Combat")
	static void SetReplicatedRotateToTargetActor(AActor* InRotatingActor, AActor* InTargetActor);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
};


