// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPGasDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "Character/TPGasBaseCharacter.h"

void UTPGasDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	float CalculatedDamage = GetCalculatedActualDamage();
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, CalculatedDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	if (DamagedActorActivateAbilityTag.IsValid())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(DamagedActorActivateAbilityTag);
			TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}

float UTPGasDamageGameplayAbility::GetCalculatedActualDamage()
{
	ATPGasBaseCharacter* OwnerCharacter = Cast<ATPGasBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwnerCharacter)) return 0.f;

	UTPGasAttributeSet* AttributeSet = Cast<UTPGasAttributeSet>(OwnerCharacter->GetAttributeSet());
	if (!IsValid(AttributeSet)) return 0.f;

	float ScaledDamage = 0.f;

	// Damage designini guncelle
	if (AbilityDamageType == EAbilityDamageType::Physical)
	{
		ScaledDamage += (AttributeSet->GetPhysicalDamage() + Damage.GetValueAtLevel(GetAbilityLevel())) * DamageScale;
	}
	else if (AbilityDamageType == EAbilityDamageType::Magic)
	{
		ScaledDamage += (AttributeSet->GetMagicalDamage() + Damage.GetValueAtLevel(GetAbilityLevel())) * DamageScale;
	}
	else // Default, Use only curve table damage
	{
		ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	}

	return ScaledDamage;
}

float UTPGasDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FString UTPGasDamageGameplayAbility::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level)); // ManaCost floati negatif geliyor diye mutlak olarak aldik.
	const float Cooldown = GetCooldown(Level);

	if (ScaledDamage > 0.f)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>%s</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost : </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number Of FireBolts
			"<Default>%s </>\n\n"

			// Damage
			"<Damage>Skill Damage : %d</>"),

			// Values
			*SkillName,
			Level,
			ManaCost,
			Cooldown,
			*SkillDescription,
			ScaledDamage);
	}
	
	return FString::Printf(TEXT(
			// Title
			"<Title>%s</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost : </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number Of FireBolts
			"<Default>%s </>\n\n"

			// Damage
			"<Damage></>"),

			// Values
			*SkillName,
			Level,
			ManaCost,
			Cooldown,
			*SkillDescription
			);
	
}

FString UTPGasDamageGameplayAbility::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level)); // ManaCost floati negatif geliyor diye mutlak olarak aldik.
	const float Cooldown = GetCooldown(Level);

	if (ScaledDamage > 0.f)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Next Level</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost : </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number Of FireBolts
			"<Default>%s </>\n\n"

			// Damage
			"<Damage>Skill Damage : %d</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			*SkillDescription,
			ScaledDamage);
	}

	return FString::Printf(TEXT(
			// Title
			"<Title>Next Level</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost : </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number Of FireBolts
			"<Default>%s </>\n\n"

			// Damage
			"<Damage></>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			*SkillDescription
			);
	
	
}


