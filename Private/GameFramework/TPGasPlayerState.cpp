// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TPGasPlayerState.h"

#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Character/TPGasCharacter.h"
#include "Net/UnrealNetwork.h"

ATPGasPlayerState::ATPGasPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UTPGasAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f; // Serverin clientleri ne siklikla updateleyecegini belirler
}

void ATPGasPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPGasPlayerState, Level);
	DOREPLIFETIME(ATPGasPlayerState, XP);
	DOREPLIFETIME(ATPGasPlayerState, SpellPoints );
	
}

UAbilitySystemComponent* ATPGasPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATPGasPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP,InXP);
}

void ATPGasPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
	if (AController* InController = Cast<AController>(GetOwner()))
	{
		if (ATPGasCharacter* PlayerCharacter = Cast<ATPGasCharacter>(InController->GetPawn()))
		{
			PlayerCharacter->Testkee(Level);
		}
	}
}

void ATPGasPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP, InXP);
}

void ATPGasPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATPGasPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATPGasPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP, XP - OldXP);
}

void ATPGasPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATPGasPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATPGasPlayerState::Server_WeaponChangeBroadcast_Implementation(bool bIsUnEquip)
{
	Multicast_WeaponChangeBroadcast(bIsUnEquip);
}

void ATPGasPlayerState::Multicast_WeaponChangeBroadcast_Implementation(bool bIsUnEquip)
{
	OnWeaponSkillChanged.Broadcast(bIsUnEquip);
}



