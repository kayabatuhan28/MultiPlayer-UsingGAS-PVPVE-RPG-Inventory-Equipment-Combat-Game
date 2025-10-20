// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPGasUserWidget.h"
#include "PlayerOnHeadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeChangedSignature, float, NewValue);

UCLASS()
class TPGAS_API UPlayerOnHeadWidget : public UTPGasUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerMaxHealthChanged;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPlayerSteamName(const FString& NewPlayerName);

	UFUNCTION(BlueprintImplementableEvent)
	void SetNatureStaffSkill4Icon(int32 InStackCount);

	
};
