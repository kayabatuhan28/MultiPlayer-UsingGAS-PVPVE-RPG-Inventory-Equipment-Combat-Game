// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPGasGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "Character/TPGasCharacter.h"
#include "GameFramework/TPGasPlayerController.h"

EInputDirection UTPGasGameplayAbility::CalculateInputDirection()
{
	//FRotator Rot = FRotator();
	ATPGasCharacter* OwningCharacter = Cast<ATPGasCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwningCharacter))
	{
		return EInputDirection::Backward;
	}
	
	ATPGasPlayerController* PlayerController = Cast<ATPGasPlayerController>(OwningCharacter->GetController());
	if (!IsValid(PlayerController))
	{
		return EInputDirection::Backward;
	}
	
	float RightLeftValue = PlayerController->ActionValueX;
	float ForwardBackwardValue = PlayerController->ActionValueY;

	if (RightLeftValue == -1) // Press A
	{
		if (ForwardBackwardValue == 1) // Press A + W
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SOL ILERI CAPRAZ"));
			return EInputDirection::ForwardLeft;
		}
		if (ForwardBackwardValue == -1) // Press A + S
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SOL GERI CAPRAZ"));
			return EInputDirection::BackwardLeft;
		}
		
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SADECE SOL"));
		return EInputDirection::Left;
	}


	if (RightLeftValue == 1) // Press D
	{
		if (ForwardBackwardValue == 1) // Press D + W
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SAG ILERI CAPRAZ"));
			return EInputDirection::ForwardRight;
		}
			
		if (ForwardBackwardValue == -1) // Press D + S
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SAG GERI CAPRAZ"));
			return EInputDirection::BackwardRight;
		}
		
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SADECE SAG"));
		return EInputDirection::Right;
	}
	
	if (ForwardBackwardValue == -1) // Press S
	{
		if ( RightLeftValue == 1) // Press S + D
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SAG GERI CAPRAZ"));
			return EInputDirection::BackwardRight;
		}

		if (RightLeftValue == -1) // Press S + A
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SOL GERI CAPRAZ"));
			return EInputDirection::BackwardLeft;
		}
		
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SADECE GERI"));
		return EInputDirection::Backward;
	}

	if (ForwardBackwardValue == 1) // Press W
	{
		if (RightLeftValue == 1) // Press W + D
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SAG ILERI CAPRAZ"));
			return EInputDirection::ForwardRight;
		}

		if (RightLeftValue == -1) // Press W + A
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SOL ILERI CAPRAZ"));
			return EInputDirection::ForwardLeft;
		}
		
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("SADECE ILERI"));
		return EInputDirection::Forward;
	}
	
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("INPUT BASILMADI SADECE GERI"));
	return EInputDirection::NoDirection;
}

void UTPGasGameplayAbility::StartStopStaminaRegen(bool bIsStart)
{
	ATPGasCharacter* OwningCharacter = Cast<ATPGasCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwningCharacter)) return;
	
	OwningCharacter->SetStaminaRegenStatus(bIsStart);
}

void UTPGasGameplayAbility::StartStopManaRegen(bool bIsStart)
{
	ATPGasCharacter* OwningCharacter = Cast<ATPGasCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwningCharacter)) return;
	
	OwningCharacter->SetManaRegenStatus(bIsStart);
}

FString UTPGasGameplayAbility::GetDescription(int32 Level)
{
	return FString("EmptyString");
}

float UTPGasGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	
	// Gasin ability librarysinden gelmekte, GetCostGameplayEffect pointer oldugu icin check etmekte fayda var
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UTPGasAttributeSet::GetManaAttribute())
			{
				// This only work if it's scalable float with either a hardcoded value or value scaled by a curve tables curve
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				// ManaCost degerini bulabilirse abilitynin cooldown float degerini setler
				break;
			}
		}
	}
	
	return ManaCost;
}

float UTPGasGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	
	return Cooldown;
}

FString UTPGasGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UTPGasGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level : %d </>"), Level);
}















	
