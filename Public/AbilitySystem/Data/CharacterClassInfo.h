// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	NoType						UMETA(DisplayName = "NoType"),
	Enemy						UMETA(DisplayName = "Enemy"),
	WareWolf					UMETA(DisplayName = "WareWolf"),
	AncientGolem				UMETA(DisplayName = "AncientGolem"),
	FireGolem					UMETA(DisplayName = "FireGolem"),
	FrankDualSword				UMETA(DisplayName = "FrankDualSword"),
	Player						UMETA(DisplayName = "Player"),
	Friend_Skeleton1			UMETA(DisplayName = "Friend_Skeleton1"),
	Friend_Skeleton2			UMETA(DisplayName = "Friend_Skeleton2"),
	Skeleton1					UMETA(DisplayName = "Skeleton1"),
	Skeleton2					UMETA(DisplayName = "Skeleton2"),
	Skeleton3					UMETA(DisplayName = "Skeleton3"),
	Boss						UMETA(DisplayName = "Boss")
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

UCLASS()
class TPGAS_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
};
