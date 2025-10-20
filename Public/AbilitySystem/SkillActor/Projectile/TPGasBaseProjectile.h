// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "TPGasBaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UGameplayEffect;
class FGameplaytag;

UCLASS()
class TPGAS_API ATPGasBaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATPGasBaseProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable)
	void ApplyDamageEffect(UAbilitySystemComponent* DamagedActorASC);
	
	UPROPERTY(BlueprintReadOnly)
	float SkillDamage = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayEffectSpecHandle DamageSpecHandle{};

	UPROPERTY(BlueprintReadOnly)
	float ProjectileLevel = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	float LifeSpanValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	FGameplayTag HittedActor_TriggeredAbilityTag{};

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	FGameplayTag DamageType{};

	UFUNCTION(BlueprintCallable)
	void SetSkillData(float InProjectileLevel, float InSkillDamage, FGameplayEffectSpecHandle InSpecHandle, FGameplayTag InHitTag, AActor* InOwner);

	// If there will be extra events at the time of hit, implement it. If not, there is no need to implement it.
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitExtra(AActor* InHittedActor);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This variable false in explosion spells, These skills deal Aoe damage equal to the environment, not impact damage.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	bool bCanApplyDamageOnOverlap = true;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(AActor* InHittedActor);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UFUNCTION(BlueprintCallable)
	bool IsValidOverlap(AActor* OtherActor);

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	bool bCanDestroyOnHit = true;

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	bool bIsEnemySkill = false;

	// Turns the actor who took the hit into the attacking actor
	UPROPERTY(EditDefaultsOnly, Category = "AData")
	bool bHitCanRotateActor = false;

private:
	
	UPROPERTY(EditAnywhere, Category = "AData")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "AData")
	FVector ImpactVfxScale = FVector(1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, Category = "AData")
	TObjectPtr<USoundBase> ImpactSound;

};
