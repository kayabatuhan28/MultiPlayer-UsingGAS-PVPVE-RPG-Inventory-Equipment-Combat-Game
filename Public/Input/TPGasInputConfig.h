// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TPGasInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FTPGasInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class TPGAS_API UTPGasInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// AbilityInputActions struct arrayinde gezer input olarak verilen tagi arar bulursa onun input actionu returnler
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTPGasInputAction> AbilityInputActions;
	
};
