// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPGasAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemLibrary.h"
#include "Character/TPGasBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/TPGasPlayerController.h"
#include "GameFramework/TPGasPlayerState.h"
#include "Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"


UTPGasAttributeSet::UTPGasAttributeSet()
{
	const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();

	// Primary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_PhysicalDamage, GetPhysicalDamageAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MagicalDamage, GetMagicalDamageAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_DamageReduction, GetDamageReductionAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_HealthRegeneration, GetHealthRegenerationAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_ManaRegeneration, GetManaRegenerationAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_AttackSpeed, GetAttackSpeedAttribute());

	// Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute());
}

void UTPGasAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, PhysicalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, MagicalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, LifeSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPGasAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UTPGasAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UTPGasAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth())); // 0'in altina inemez veya maxhealth üstüne cikamaz.
		//UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health : %f"),* Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() >= GetMaxHealth())
		{
			SetHealth(GetMaxHealth());
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}

	if (Data.EvaluatedData.Attribute == GetDamageReductionAttribute())
	{
		SetDamageReduction(FMath::Clamp(GetDamageReduction(), 0.f, 70.f));
	}

	if (Data.EvaluatedData.Attribute == GetCriticalHitChanceAttribute())
	{
		SetCriticalHitChance(FMath::Clamp(GetCriticalHitChance(), 0.f, 100.f));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
	{
		float LocalIncomingHeal = GetIncomingHeal();
		SetIncomingHeal(0.f);
		
		if (LocalIncomingHeal > 0.f)
		{
			const float NewHealth = GetHealth() + LocalIncomingHeal;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			ShowFloatingText(Props, LocalIncomingHeal, true);
		}
	}
}

void UTPGasAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source : Effecte sebep olan, Target : Effectten etkilenen (Bu AttributeSetin Owneri, yani bu class)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}


	// Target İcin

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UTPGasAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bIsHealing) const
{
	if (ATPGasBaseCharacter* BaseCharacter = Cast<ATPGasBaseCharacter>(Props.SourceCharacter))
	{
		BaseCharacter->ShowDamageNumber(Damage, Props.TargetCharacter, bIsHealing, BaseCharacter->bIsCriticalHit);
		if (BaseCharacter->bIsCriticalHit == true)
		{
			BaseCharacter->bIsCriticalHit = false;
		}
	}
}

void UTPGasAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	
}

void UTPGasAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();

	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag DieTag = Tags.Abilities_Die;

	if (IsValid(Props.TargetASC))
	{
		if (Props.TargetASC->HasMatchingGameplayTag(DieTag))
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green, TEXT("HandleIncomingDamage Early Exit!!"));
			return;
		}
	}
	
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		
		if (ATPGasBaseCharacter* BaseCharacter = Cast<ATPGasBaseCharacter>(Props.SourceAvatarActor)) // Damage Causer healing
		{
			UTPGasAbilitySystemLibrary::LifeSteal(BaseCharacter, BaseCharacter->GE_Heal, LocalIncomingDamage);
		}
		
		const bool bFatal = NewHealth <= 0.f; 
		if (bFatal)
		{
			IICombatInterface* CombatInterface = Cast<IICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				const FVector Impulse = FVector::ZeroVector;
				CombatInterface->Die(Impulse);
			}
			SendXPEvent(Props);
		}
		else
		{
			// Add HitReact, Hit vfx or something.
		}

		ShowFloatingText(Props, LocalIncomingDamage, false);
	}
}

void UTPGasAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Handle Incoming XP Girdi"));
	
	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UICombatInterface>())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Handle Incoming XP ife Girdi"));
		const int32 CurrentLevel = IICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for (int32 i = 0; i <NumLevelUps; ++i)
			{
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
				//AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			
			//IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			//bTopOffHealth = true;
			//bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UTPGasAttributeSet::SendXPEvent(const FEffectProperties& InProps)
{
	if (InProps.TargetCharacter->Implements<UICombatInterface>())
	{
		
		
		// Blueprint native eventi direk CombatInterfaceden cagirdigimiz icin çökme olmaktaydı bu yüzden execute versiyonunu cagırdık  direk 
		// combatınterfaceden cagırmak yerine
		const int32 TargetLevel = IICombatInterface::Execute_GetPlayerLevel(InProps.TargetCharacter);
		const ECharacterClass TargetClass = IICombatInterface::Execute_GetCharacterClass(InProps.TargetCharacter);
		const int32 XPReward = UTPGasAbilitySystemLibrary::GetXPRewardForClassAndLevel(InProps.TargetCharacter, TargetClass, TargetLevel);

		const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();
		
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;

		// Ai ya saldırana yani playera bu eventi yollarız.
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InProps.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}


/*
float UTPGasAttributeSet::CalculateActualDamage(const FEffectProperties& Props, float InComingDamage)
{
	if (!IsValid(Props.TargetCharacter)) return 0.f;

	ATPGasBaseCharacter* TargetCharacter = Cast<ATPGasBaseCharacter>(Props.TargetCharacter);
	if (!IsValid(TargetCharacter)) return 0.f;

	UTPGasAttributeSet* TargetAttributeSet = Cast<UTPGasAttributeSet>(TargetCharacter->GetAttributeSet());
	if (!IsValid(TargetAttributeSet)) return 0.f;

	float LocalDamageReduction = TargetAttributeSet->GetDamageReduction(); 
	float LocalDamageReductionAmount = InComingDamage * (LocalDamageReduction / 100.f);

	float CalculatedDamage = InComingDamage - LocalDamageReductionAmount;
	return CalculatedDamage;
}
*/

void UTPGasAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, Health, OldHealth);
}

void UTPGasAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, Mana, OldMana);
}

void UTPGasAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, Stamina, OldStamina);
}

void UTPGasAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, Vigor, OldVigor);
}

void UTPGasAttributeSet::OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, PhysicalDamage, OldPhysicalDamage);
}

void UTPGasAttributeSet::OnRep_MagicalDamage(const FGameplayAttributeData& OldMagicalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, MagicalDamage, OldMagicalDamage);
}

void UTPGasAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, DamageReduction, OldDamageReduction);
}

void UTPGasAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UTPGasAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UTPGasAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
}

void UTPGasAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UTPGasAttributeSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, LifeSteal, OldLifeSteal);
}

void UTPGasAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UTPGasAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UTPGasAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, MaxHealth, OldMaxHealth);
}

void UTPGasAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, MaxStamina, OldMaxStamina);
}

void UTPGasAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPGasAttributeSet, MaxMana, OldMaxMana);
}


