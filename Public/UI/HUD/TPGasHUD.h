// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemComponent.h"
#include "TPGasHUD.generated.h"

class UTPGasUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class USpellMenuWidgetController;

UCLASS()
class TPGAS_API ATPGasHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InıtOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UFUNCTION()
	void TryInitOverlay(); // Retry mekanizması

	UPROPERTY()
	TObjectPtr<UTPGasUserWidget> StatMenuWidget; // Main ana widget olmasi icin olusturduk

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPGasUserWidget> StatMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UTPGasUserWidget> SkillMenuWidget; // Main ana widget olmasi icin olusturduk

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPGasUserWidget> SkillMenuWidgetClass;

private:

	UPROPERTY()
	TObjectPtr<UTPGasUserWidget> OverlayWidget; // Main ana widget olmasi icin olusturduk

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPGasUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	
	
};
