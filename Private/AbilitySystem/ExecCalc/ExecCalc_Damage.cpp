// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "Character/TPGasBaseCharacter.h"
#include "Interface/ICombatInterface.h"

struct TPGasDamageStatics
{
	// Bu makro ile ModMagCalc'ta yaptigimiz gibi farkli bir yolla attribute captureleyebiliriz. Attribute isimlerini yazarız
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	
	
	TPGasDamageStatics()
	{
		// Damage Causer = Source   Damaged Actor = Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPGasAttributeSet, DamageReduction, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPGasAttributeSet, Stamina, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPGasAttributeSet, CriticalHitChance, Source, false) 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPGasAttributeSet, CriticalHitDamage, Source, false) 
	}
};

static const TPGasDamageStatics& DamageStatics()
{
	static TPGasDamageStatics DStatics;
	return DStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatics().StaminaDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();

	TagsToCaptureDefs.Add(Tags.Attributes_Primary_DamageReduction, DamageStatics().DamageReductionDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Vital_Stamina, DamageStatics().StaminaDef);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (!IsValid(SourceASC) || !IsValid(TargetASC))
	{
		return;
	}

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	if (SourceAvatar == nullptr || TargetAvatar == nullptr)
	{
		return;
	}

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UICombatInterface>())
	{
		SourcePlayerLevel = IICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UICombatInterface>())
	{
		TargetPlayerLevel = IICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Stamina
	float Stamina = 0.f;
	const FGameplayTag StaminaTag = Tags.Attributes_Vital_Stamina;

	const FGameplayEffectAttributeCaptureDefinition StaminaDef = TagsToCaptureDefs[StaminaTag];
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StaminaDef, EvaluationParameters, Stamina);

	// Damage
	float Damage = 0;

	const FGameplayTag DamageTypeTag = Tags.Damage;
	float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
	
	// Damage Reduction Calculate
	float DamageReduction = 0.f;
	const FGameplayTag DamageReductionTag = Tags.Attributes_Primary_DamageReduction;
	
	const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[DamageReductionTag];
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, DamageReduction);
	DamageReduction = FMath::Clamp(DamageReduction, 0.f, 100.f); // Resistance % li isleyecegi icin kısıtlama ekledik.

	DamageTypeValue *= (100.f - DamageReduction) / 100.f;

	// Set Calculated Damage
	Damage += DamageTypeValue;

	// Calculate Critical Hit Chance And Critical Hit Damage
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	bool bIsCriticalHit = FMath::RandRange(0, 100) <= SourceCriticalHitChance;
	if (bIsCriticalHit)
	{
		if (ATPGasBaseCharacter* SourceCharacter = Cast<ATPGasBaseCharacter>(SourceAvatar))
		{
			Damage *= SourceCriticalHitDamage;
			SourceCharacter->bIsCriticalHit = true;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("CRİTİCAL HİT DAMAGE!!!"));
		}
	}

	// Guarding Check
	bool bIsDamageBlocked = false;
	if (TargetASC->HasMatchingGameplayTag(Tags.Abilities_Combat_Guarding))
	{
		bIsDamageBlocked = IsDamageBlockedWithGuarding(SourceASC, TargetASC, Stamina, Damage);
	}
	
	if (bIsDamageBlocked == false)
	{
		const FGameplayModifierEvaluatedData EvaluatedData(UTPGasAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
	}
	else
	{
		const FGameplayModifierEvaluatedData DamageEvaluatedData(UTPGasAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, 0);
		OutExecutionOutput.AddOutputModifier(DamageEvaluatedData);

		const FGameplayModifierEvaluatedData StaminaEvaluatedData(UTPGasAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -Damage * 2.f);
		OutExecutionOutput.AddOutputModifier(StaminaEvaluatedData);
	}
	
	
}

bool UExecCalc_Damage::IsDamageBlockedWithGuarding(const UAbilitySystemComponent* SourceASC,
	const UAbilitySystemComponent* TargetASC,  const float SourceStamina, const float ConsumedStamina) const
{
	if (SourceStamina - ConsumedStamina <= 0.f)
	{
		// Send Message Block Broken, get half damage
		FGameplayEventData Payload;
		Payload.Instigator = SourceASC->GetAvatarActor();
		const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Event.Combat.BlockBroken");
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), EventTag, Payload);
		return false;
	}

	// Send Message All Damage blocked and play hit block montage
	FGameplayEventData Payload;
	Payload.Instigator = SourceASC->GetAvatarActor();
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Event.Combat.BlockHitted");
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), EventTag, Payload);
	return true;
}
