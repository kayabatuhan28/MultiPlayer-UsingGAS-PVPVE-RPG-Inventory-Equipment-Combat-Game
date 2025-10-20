// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/TPGasWidgetController.h"

#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "GameFramework/TPGasPlayerController.h"
#include "GameFramework/TPGasPlayerState.h"

void UTPGasWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTPGasWidgetController::BroadcastInitialValues()
{
	
}

void UTPGasWidgetController::BindCallBacksDependencies()
{
	
}

void UTPGasWidgetController::BroadcastAbilityInfo()
{
	if(!GetTPGasASC()->bStartupAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FTPGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TPGasAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = TPGasAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = TPGasAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
		
	});
	GetTPGasASC()->ForEachAbility(BroadcastDelegate);
	
}

ATPGasPlayerController* UTPGasWidgetController::GetTPGasPC()
{
	if (TPGasPlayerController == nullptr)
	{
		TPGasPlayerController = Cast<ATPGasPlayerController>(PlayerController);
	}
	return TPGasPlayerController;
}

ATPGasPlayerState* UTPGasWidgetController::GetTPGasPS()
{
	if (TPGasPlayerState == nullptr)
	{
		TPGasPlayerState = Cast<ATPGasPlayerState>(PlayerState);
	}
	return TPGasPlayerState;
}

UTPGasAbilitySystemComponent* UTPGasWidgetController::GetTPGasASC()
{
	if (TPGasAbilitySystemComponent == nullptr)
	{
		TPGasAbilitySystemComponent = Cast<UTPGasAbilitySystemComponent>(AbilitySystemComponent);
	}
	return TPGasAbilitySystemComponent;
}

UTPGasAttributeSet* UTPGasWidgetController::GetTPGasAS()
{
	if (TPGasAttributeSet == nullptr)
	{
		TPGasAttributeSet = Cast<UTPGasAttributeSet>(AttributeSet);
	}
	return TPGasAttributeSet;
}
