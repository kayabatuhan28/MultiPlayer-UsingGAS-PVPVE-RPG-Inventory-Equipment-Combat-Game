// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	if (const FCharacterClassDefaultInfo* Info = CharacterClassInformation.Find(CharacterClass))
	{
		return *Info;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	//	FString::Printf(TEXT("CharacterClass '%s' not found in CharacterClassInformation!"), *UEnum::GetValueAsString(CharacterClass)));
	
	return FCharacterClassDefaultInfo(); // Default boş struct döndür
}
