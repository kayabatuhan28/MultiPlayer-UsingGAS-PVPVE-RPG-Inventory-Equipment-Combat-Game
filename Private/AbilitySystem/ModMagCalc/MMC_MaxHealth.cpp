// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "Interface/ICombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UTPGasAttributeSet::GetVigorAttribute(); // Attribute Accessors sayesinde bu sekilde erisebildik.
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false; // True olursa capturing the attribute right away as soon as the effect spec is created

	RelevantAttributesToCapture.Add(VigorDef); // MMC Capture defination arrayine ekler.
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor); // Vigor degerini yakalar
	Vigor = FMath::Max<float>(Vigor, 0); // Vigor 0 dan kücükse 0 setler.
	
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UICombatInterface>())
	{
		//PlayerLevel = IICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
		PlayerLevel = 1;
	}

	return 50.f + Vigor * 2.f;
	
}
