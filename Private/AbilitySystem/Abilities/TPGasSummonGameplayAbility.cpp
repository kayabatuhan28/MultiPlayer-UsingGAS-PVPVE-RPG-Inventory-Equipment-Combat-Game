// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPGasSummonGameplayAbility.h"

#include "Character/TPGasCharacter.h"
#include "Character/TPGasEnemyCharacter.h"

void UTPGasSummonGameplayAbility::SpawnSummonActor(TSubclassOf<AActor> SummonedActor, FVector SpawnLocation, FRotator SpawnRotation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	
	// Set the Projectile Rotation
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AActor* SpawnedSummonActor = GetWorld()->SpawnActorDeferred<AActor>(
	SummonedActor,
	SpawnTransform,
	GetOwningActorFromActorInfo(),
	Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (ATPGasEnemyCharacter* SpawnedAi = Cast<ATPGasEnemyCharacter>(SpawnedSummonActor))
	{
		SpawnedAi->AiMasterActor = GetAvatarActorFromActorInfo();
		SpawnedAi->Level = GetAbilityLevel();
		if (ATPGasCharacter* AbilityOwnerPlayer = Cast<ATPGasCharacter>(GetAvatarActorFromActorInfo()))
		{
			AbilityOwnerPlayer->SummonedActors.Add(SpawnedAi);
		}
	}

	
	SpawnedSummonActor->FinishSpawning(SpawnTransform);

	
}

FString UTPGasSummonGameplayAbility::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level)); // ManaCost floati negatif geliyor diye mutlak olarak aldik.
	const float Cooldown = GetCooldown(Level);
	
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

FString UTPGasSummonGameplayAbility::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level)); // ManaCost floati negatif geliyor diye mutlak olarak aldik.
	const float Cooldown = GetCooldown(Level);
	
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
			"<Damage>Increases the damage and health of skeletons </>"),

			// Values
			*SkillName,
			Level,
			ManaCost,
			Cooldown,
			*SkillDescription
			);
}
