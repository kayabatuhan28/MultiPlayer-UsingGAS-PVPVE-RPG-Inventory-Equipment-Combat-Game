// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/TPGasGameMode.h"
#include "Character/TPGasCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPGasGameMode::ATPGasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
