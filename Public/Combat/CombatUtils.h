#pragma once

#include "CombatUtils.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	UnEquipped,
	TwoHandedAxe,
	OneHandedSword,
	SwordAndShield,
	Spear,
	Bow,
	Staff
};

UENUM(BlueprintType)
enum class EInputDirection : uint8
{
	NoDirection,
	Backward,
	BackwardLeft,
	BackwardRight,
	Forward,
	ForwardLeft,
	ForwardRight,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EHitDirection: uint8
{
	NoDirection,
	Forward,
	Backward
};

USTRUCT(BlueprintType)
struct FCombatAnimationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	TArray<UAnimMontage*> LightAttackMontages{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	TArray<UAnimMontage*> HeavyAttackMontages{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	TArray<UAnimMontage*> BasicHitMontage{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	TArray<UAnimMontage*> HeavyHitMontage{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	TArray<UAnimMontage*> StunMontage{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat/Animation")
	UAnimMontage* AfterBackwardDodgeAttack{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat/Animation")
	UAnimMontage* AfterForwardDodgeAttack{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat/Animation")
	EWeaponType WeaponType = EWeaponType::UnEquipped;
	
};

USTRUCT(BlueprintType)
struct FCombatData
{
	GENERATED_BODY()

	FCombatData() {}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainData")
	FCombatAnimationData CombatAnimationData{};

	
};