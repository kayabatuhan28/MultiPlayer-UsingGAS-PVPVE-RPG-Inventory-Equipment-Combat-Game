// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SkillActor/Projectile/TPGasBaseProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemLibrary.h"
#include "Character/TPGasEnemyCharacter.h"
#include "GameFramework/TPGasPlayerState.h"


// Sets default values
ATPGasBaseProjectile::ATPGasBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	//Sphere->SetCollisionObjectType(ECC_Projectile); // Aura.h teki defineden geldi
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ATPGasBaseProjectile::ApplyDamageEffect(UAbilitySystemComponent* DamagedActorASC)
{
	
	const AActor* DamagedActor = DamagedActorASC->GetAvatarActor();

	FGameplayEffectContextHandle ContextHandle = DamagedActorASC->MakeEffectContext();
	ContextHandle.AddSourceObject(DamagedActor);

	//FGameplayEffectSpecHandle SpecHandle = DamagedActorASC->MakeOutgoingSpec(DamageEffectClass, ProjectileLevel, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, SkillDamage);

	DamagedActorASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), DamagedActorASC);
}

void ATPGasBaseProjectile::SetSkillData(float InProjectileLevel, float InSkillDamage, FGameplayEffectSpecHandle InSpecHandle, FGameplayTag InHitTag, AActor* InOwner)
{
	ProjectileLevel = InProjectileLevel;
	SkillDamage = InSkillDamage;
	DamageSpecHandle = InSpecHandle;
	HittedActor_TriggeredAbilityTag = InHitTag;
	SetOwner(InOwner);
}

// Called when the game starts or when spawned
void ATPGasBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpanValue);
	
	SetReplicateMovement(true);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATPGasBaseProjectile::OnSphereOverlap);
	
}

void ATPGasBaseProjectile::OnHit(AActor* InHittedActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UGameplayStatics::SpawnEmitterAtLocation(
		this,
		ImpactEffect,
		GetActorLocation(),
		FRotator::ZeroRotator,
		ImpactVfxScale,
		true,
		EPSCPoolMethod::None,
		true
		);

	
	//LoopingSoundComponent->Stop();
	/* Bu kısım kapatıldı - bazı sikintilar vardi?
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	*/
	bHit = true;
	OnHitExtra(InHittedActor);
}

void ATPGasBaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit(OtherActor);
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (bCanApplyDamageOnOverlap == true)
			{
				ApplyDamageEffect(TargetASC);
				if (HittedActor_TriggeredAbilityTag.IsValid())
				{
					if (bHitCanRotateActor == true)
					{
						UTPGasAbilitySystemLibrary::SetReplicatedRotateToTargetActor(OtherActor, this);
					}
					FGameplayTagContainer TagContainer;
					TagContainer.AddTag(HittedActor_TriggeredAbilityTag);
					TargetASC->TryActivateAbilitiesByTag(TagContainer);
				}
			}
		}
		if (bCanDestroyOnHit == true)
		{
			Destroy();
		}
	}
	else bHit = true;
}

bool ATPGasBaseProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (bIsEnemySkill == false)
	{
		const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();
		FGameplayTag PvpDisableTag = GameplayTags.Effects_PvpDisabled;
	
		UAbilitySystemComponent* OtherActorASC = Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor));
		if (OtherActorASC == nullptr)
		{
			return false;
		}

		if (GetOwner() == OtherActor)
		{
			return false;
		}

		if (OtherActorASC->HasMatchingGameplayTag(PvpDisableTag))
		{
			return false;
		}

		if (OtherActor->ActorHasTag("FriendSummon"))
		{
			FGameplayTag IsSummonFriendTag = GameplayTags.Effects_Ai_SummonIsFriend;
			if (OtherActorASC->HasMatchingGameplayTag(IsSummonFriendTag)) // Players pvp off, players skill cant damage summoned ai
			{
				return false;
			}

			// Player pvp enable, Summoned ai owner cant hit summoned ai but other players can hit.
			if (ATPGasEnemyCharacter* EnemyCharacter = Cast<ATPGasEnemyCharacter>(OtherActor))
			{
				if (EnemyCharacter->AiMasterActor == GetOwner()) // Summoned ai owner try hit the summoned ai
				{
					return false;
				}
			}
		}

		return true;
	}

	// Enemy Skill

	UAbilitySystemComponent* OtherActorASC = Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor));
	if (OtherActorASC == nullptr)
	{
		return false;
	}

	if (GetOwner() == OtherActor)
	{
		return false;
	}

	if (OtherActor->ActorHasTag("Enemy") && !OtherActor->ActorHasTag("FriendSummon") )
	{
		return false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy Skill Valid Overlap Trueke!!"));
	return true;
	
}


