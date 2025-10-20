// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPGasGameMode.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

UCLASS(minimalapi)
class ATPGasGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPGasGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	
};



