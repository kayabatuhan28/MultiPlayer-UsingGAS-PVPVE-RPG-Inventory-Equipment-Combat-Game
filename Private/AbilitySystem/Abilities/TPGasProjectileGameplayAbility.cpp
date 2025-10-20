// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPGasProjectileGameplayAbility.h"

#include "AbilitySystem/SkillActor/Projectile/TPGasBaseProjectile.h"
#include "Camera/CameraComponent.h"
#include "Character/TPGasCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTPGasProjectileGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTPGasProjectileGameplayAbility::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	
	// Set the Projectile Rotation
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	ATPGasBaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATPGasBaseProjectile>(
	ProjectileClass,
	SpawnTransform,
	GetOwningActorFromActorInfo(),
	Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	Projectile->ProjectileLevel = GetAbilityLevel();
	Projectile->SkillDamage = GetCalculatedActualDamage();
	Projectile->DamageSpecHandle = DamageSpecHandle;
	Projectile->HittedActor_TriggeredAbilityTag = DamagedActorActivateAbilityTag;
	Projectile->SetOwner(GetAvatarActorFromActorInfo());
	
	Projectile->FinishSpawning(SpawnTransform);
}

FVector UTPGasProjectileGameplayAbility::GetAimImpactPoint()
{
	ATPGasCharacter* OwnerPlayer = Cast<ATPGasCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwnerPlayer)) return FVector::ZeroVector;
	
	FVector OutImpactPoint;

	float ForwardValue = 8000.f;
	FVector TraceStart = OwnerPlayer->GetFollowCamera()->GetComponentLocation();
	FVector TraceEnd = OwnerPlayer->GetFollowCamera()->GetComponentLocation() + (OwnerPlayer->GetFollowCamera()->GetForwardVector() * ForwardValue);
	
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (HitResult.bBlockingHit)
	{
		OutImpactPoint = HitResult.ImpactPoint;
	}
	else
	{
		OutImpactPoint = HitResult.TraceEnd;	
	}

	/*
	DrawDebugLine(GetWorld(),
		TraceStart,
		TraceEnd,
		HitResult.bBlockingHit ? FColor::Green : FColor::Red,
		false,
		5.0f,
		0,
		0.f);
	*/	

	return OutImpactPoint;
	
}
