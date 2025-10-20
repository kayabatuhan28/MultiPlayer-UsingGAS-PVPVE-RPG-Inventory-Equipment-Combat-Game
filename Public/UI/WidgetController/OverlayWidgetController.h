// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatUtils.h"
#include "UI/WidgetController/TPGasWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignatureTwoParams, float, NewValue, float, SecondValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeapoUIDataChanged, bool, bIsUnEquip);

UCLASS(BlueprintType,  Blueprintable)
class TPGAS_API UOverlayWidgetController : public UTPGasWidgetController
{
	GENERATED_BODY()

public:

	// Baslangicta widget datasi setlemek icin olusturdk widgetlara broadcast yapar, 
	virtual void BroadcastInitialValues() override; 

	// Gastan, abilitycomptan gelen AttributeChanged delegateleri baglamak icin kullandik, AuraHud.cpp de kullandik.
	virtual void BindCallBacksDependencies() override;

	// Health Globe widgetlarında bu delegateleri bağladık, bu delegateleride globe widgetlarda assignlariz
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnDamageReductionChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnPhysicalDamageChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMagicalDamageChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthRegenChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaRegenChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaRegenChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnLifeStealChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnCriticalHitChanceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnCriticalHitDamageChanged;
	

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignatureTwoParams OnXpChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SkillUI")
	FOnWeapoUIDataChanged OnWeapoUIDataChanged;

	void OnWeaponSkillChanged(bool bIsUnEquip);

	
	
protected:

	void OnXPChanged(int32 NewXP, int32 GainedXP);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
	

	
};





