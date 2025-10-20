// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/TPGasPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetTPGasAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetTPGasAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetTPGasAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetTPGasAS()->GetMaxMana());
	OnStaminaChanged.Broadcast(GetTPGasAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetTPGasAS()->GetMaxStamina());

	if(UTPGasAbilitySystemComponent* AuraASC = Cast<UTPGasAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
	}
}

void UOverlayWidgetController::BindCallBacksDependencies()
{
	GetTPGasPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	GetTPGasPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	GetTPGasPS()->OnWeaponSkillChanged.AddLambda([this](const bool bIsUnEquip)
	{
		OnWeaponSkillChanged(bIsUnEquip);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetMaxStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetDamageReductionAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnDamageReductionChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetPhysicalDamageAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnPhysicalDamageChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetMagicalDamageAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMagicalDamageChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetHealthRegenerationAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthRegenChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetManaRegenerationAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaRegenChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetStaminaRegenerationAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaRegenChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetLifeStealAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnLifeStealChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetCriticalHitChanceAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnCriticalHitChanceChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTPGasAS()->GetCriticalHitDamageAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnCriticalHitDamageChanged.Broadcast(Data.NewValue);
			}
		);

	if (GetTPGasASC())
	{
		GetTPGasASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetTPGasASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetTPGasASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP, int32 GainedXP)
{
	const ULevelUpInfo* LevelUpInfo = GetTPGasPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState blueprint"));
	
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				//FString::Printf(TEXT("New Player XP : %d"), NewXP));

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		//	FString::Printf(TEXT("Level Requirement : %d,   PreviousLevelRequirement : %d"), LevelUpRequirement, PreviousLevelUpRequirement));

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		//	FString::Printf(TEXT("XPForThisLevel : %d,   DeltaLevelRequirement : %d"), XPForThisLevel, DeltaLevelRequirement));

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
		//OnXpChangedDelegate.Broadcast(NewXP, LevelUpRequirement);
		OnXpChangedDelegate.Broadcast(XPForThisLevel, DeltaLevelRequirement);
		if (TPGasPlayerState)
		{
			TPGasPlayerState->XPGain(GainedXP, NewXP, LevelUpRequirement);
		}
	}
}

void UOverlayWidgetController::OnWeaponSkillChanged(bool bIsUnEquip)
{
	OnWeapoUIDataChanged.Broadcast(bIsUnEquip);
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();

	// Clear the old slot
	FTPGasAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_SkillStatus_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if Previous Slot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill the new slot
	FTPGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("OverlayWidgetController.cpp OnAbilityEquippeda Girdi!"));
}
