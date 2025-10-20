// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPGasBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/DamageTextComponent.h"

// Sets default values
ATPGasBaseCharacter::ATPGasBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ATPGasBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ATPGasBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPGasBaseCharacter, bIsFlameThrowing);
}



FOnDeathSignature& ATPGasBaseCharacter::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

FOnDamageSignature& ATPGasBaseCharacter::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

ECharacterClass ATPGasBaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

AActor* ATPGasBaseCharacter::GetAvatar_Implementation()
{
	return this;
}

FOnASCRegistered& ATPGasBaseCharacter::GetOnAscRegisteredDelegate()
{
	return OnAscRegistered;
}

void ATPGasBaseCharacter::Die(const FVector& DeathImpulse)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(ClearTagWhenDie);
	}
	
	MulticastHandleDeath(DeathImpulse);
}

void ATPGasBaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	bDead = true;
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Duvar, floor vs gibi seylere response verebilmesi icin
	GetMesh()->AddImpulse(DeathImpulse, "Spine_01", true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ATPGasBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

void ATPGasBaseCharacter::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsHealing,  bool bInIsCriticalHit)
{
	ShowDamageNumber_Multicast(DamageAmount, TargetCharacter, bIsHealing, bInIsCriticalHit);
}

void ATPGasBaseCharacter::ShowDamageNumber_Multicast_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsHealing,  bool bInIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		
		// Constructorda CreateDefaultSubobject dedigimiz zaman bu kısım otomatik olur ama constructta componenti attachlemedigimiz icin register ettik
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		// Widget animasyonu vurdugumuz yerde kalması icin hemen attachledikten sonra detachledik
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageText(DamageAmount, bIsHealing, bInIsCriticalHit);
	}
}

void ATPGasBaseCharacter::SetActorRotationReplicated(FRotator NewRot)
{
	Server_SetActorRotationReplicated(NewRot);
}

void ATPGasBaseCharacter::Server_SetActorRotationReplicated_Implementation(FRotator NewRot)
{
	Multicast_SetActorRotationReplicated(NewRot);
}

void ATPGasBaseCharacter::Multicast_SetActorRotationReplicated_Implementation(FRotator NewRot)
{
	SetActorRotation(NewRot);
}

void ATPGasBaseCharacter::SetMovementSpeedReplicated(float NewSpeed)
{
	Server_SetMovementSpeed(NewSpeed);
}

void ATPGasBaseCharacter::Server_SetMovementSpeed_Implementation(float NewSpeed)
{
	Multicast_SetMovementSpeed(NewSpeed);
}

void ATPGasBaseCharacter::Multicast_SetMovementSpeed_Implementation(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ATPGasBaseCharacter::BurningTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	
}

void ATPGasBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATPGasBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPGasBaseCharacter::InitAbilityActorInfo()
{
	
}

void ATPGasBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent()); 
}

void ATPGasBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ATPGasBaseCharacter::AddCharacterAbilities()
{
	UTPGasAbilitySystemComponent* TPGasASC = CastChecked<UTPGasAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	TPGasASC->AddCharacterAbilities(StartupAbilities, 1);
	TPGasASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}




