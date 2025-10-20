// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TPGasHUD.h"

#include "GameFramework/TPGasPlayerState.h"
#include "UI/Widget/TPGasUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* ATPGasHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacksDependencies(); 
	}
	return OverlayWidgetController;
}

USpellMenuWidgetController* ATPGasHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallBacksDependencies();
	}
	return SpellMenuWidgetController;
}

void ATPGasHUD::InıtOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (!PC || !PS || !ASC || !AS)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD::InitOverlay - invalid params"));
		return;
	}

	if (!OverlayWidgetClass || !OverlayWidgetControllerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD::InitOverlay - widget classes not set"));
		return;
	}

	// Widget yaratma
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTPGasUserWidget>(Widget);

	UUserWidget* Widget2 = CreateWidget<UUserWidget>(GetWorld(), StatMenuWidgetClass);
	StatMenuWidget = Cast<UTPGasUserWidget>(Widget2);

	UUserWidget* Widget3 = CreateWidget<UUserWidget>(GetWorld(), SkillMenuWidgetClass);
	SkillMenuWidget = Cast<UTPGasUserWidget>(Widget3);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	if (!WidgetController || !OverlayWidget || !StatMenuWidget || !SkillMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD::InitOverlay - failed to create widgets/controllers"));
		return;
	}

	OverlayWidget->SetWidgetController(WidgetController);
	StatMenuWidget->SetWidgetController(WidgetController);
	SkillMenuWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();

	// Add to viewport
	OverlayWidget->AddToViewport();

	StatMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	StatMenuWidget->AddToViewport();

	SkillMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	SkillMenuWidget->AddToViewport();

	UE_LOG(LogTemp, Log, TEXT("HUD::InitOverlay - UI successfully initialized"));
	
}

void ATPGasHUD::TryInitOverlay()
{
	APlayerController* PC = GetOwningPlayerController();
	ATPGasPlayerState* PS = PC ? PC->GetPlayerState<ATPGasPlayerState>() : nullptr;
	UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
	UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;

	if (PC && PS && ASC && AS)
	{
		InıtOverlay(PC, PS, ASC, AS);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
		//	TEXT("HUD::TryInitOverlay - dependencies not ready, retrying next tick"));
		
		// 1 tick sonra tekrar dene
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATPGasHUD::TryInitOverlay);
	}
}
