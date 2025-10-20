// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TPGasPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/SkillActor/TargetArea.h"
#include "Camera/CameraComponent.h"
#include "Character/TPGasCharacter.h"
#include "InventorySystem/Widgets/HUD/Inv_HUDWidget.h"
#include "InventorySystem/Interaction/Inv_Highlightable.h"
#include "Input/TPGasInputComponent.h"
#include "InventorySystem/InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventorySystem/Items/Components/Inv_ItemComponent.h"
#include "InventorySystem/Items/ItemActors/Inv_ChestActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/TPGasHUD.h"
#include "UI/Widget/TPGasUserWidget.h"

ATPGasPlayerController::ATPGasPlayerController()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.f;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void ATPGasPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
	
}

void ATPGasPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPGasPlayerController, ActionValueX);
	DOREPLIFETIME(ATPGasPlayerController, ActionValueY);
}

void ATPGasPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext); // Fail gelirse crash gelir

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();
	CreateHudWidget();
	
	
}

void ATPGasPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UTPGasInputComponent* TPGasInputComponent = CastChecked<UTPGasInputComponent>(InputComponent); // Cast failse crash yer
	
	TPGasInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPGasPlayerController::Move);
	TPGasInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATPGasPlayerController::MoveCompleted);
	TPGasInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPGasPlayerController::Look);
	

	TPGasInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ATPGasPlayerController::PrimaryInteract);
	TPGasInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ATPGasPlayerController::ToggleInventory);
	TPGasInputComponent->BindAction(OpenStatMenuAction, ETriggerEvent::Started, this, &ATPGasPlayerController::ToggleStatMenu);
	TPGasInputComponent->BindAction(OpenSkillMenuAction, ETriggerEvent::Started, this, &ATPGasPlayerController::ToggleSkillMenu);
	
	TPGasInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATPGasPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ATPGasPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void ATPGasPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

UTPGasAbilitySystemComponent* ATPGasPlayerController::GetASC()
{
	if (TPGasAbilitySystemComponent == nullptr)
	{
		TPGasAbilitySystemComponent = Cast<UTPGasAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TPGasAbilitySystemComponent;
}

void ATPGasPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
		
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	Server_SetInputActionValues(MovementVector.X, MovementVector.Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPGasPlayerController::MoveCompleted()
{
	Server_SetInputActionValues(0,0);
}

void ATPGasPlayerController::Server_SetInputActionValues_Implementation(float InActionValueX, float InActionValueY)
{
	Multicast_SetInputActionValues(InActionValueX, InActionValueY);
}

void ATPGasPlayerController::Multicast_SetInputActionValues_Implementation(float InActionValueX, float InActionValueY)
{
	ActionValueX = InActionValueX;
	ActionValueY = InActionValueY;
}

void ATPGasPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Inventory System
void ATPGasPlayerController::PrimaryInteract()
{
	if (InteractableChest)
	{
		//InteractableChest->ChestOpened(GetPawn());
		Server_RequestOpenChest(InteractableChest);
	}
	else
	{
		if (!ThisActor.IsValid()) return;

		UInv_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

		InventoryComponent->TryAddItem(ItemComp);
	}
}

void ATPGasPlayerController::Server_RequestOpenChest_Implementation(AInv_ChestActor* ChestRef)
{
	if (ChestRef)
	{
		ChestRef->ChestOpened(GetPawn());
	}
}

void ATPGasPlayerController::CreateHudWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HudWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void ATPGasPlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();

	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void ATPGasPlayerController::ToggleStatMenu()
{
	ATPGasHUD* HUD = Cast<ATPGasHUD>(GetHUD());
	if (!IsValid(HUD)) return;

	if (bIsOpenStatMenuOpen == true)
	{
		HUD->StatMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		bIsOpenStatMenuOpen = false;
	}
	else
	{
		HUD->StatMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bIsOpenStatMenuOpen = true;
	}
}

void ATPGasPlayerController::ToggleSkillMenu()
{
	ATPGasHUD* HUD = Cast<ATPGasHUD>(GetHUD());
	if (!IsValid(HUD)) return;

	if (bIsSkillMenuOpen == true)
	{
		HUD->SkillMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		bIsSkillMenuOpen = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}
	else
	{
		HUD->SkillMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bIsSkillMenuOpen = true;

		FInputModeGameAndUI InputModeData;
		//InputModeData.SetWidgetToFocus(HUD->SkillMenuWidget->TakeWidget());
		//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputModeData);
		SetShowMouseCursor(true);
		
	}
}

void ATPGasPlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);
	
	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
	}

	if (ThisActor == LastActor) return;
		
	if (ThisActor.IsValid())
	{
		if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
		{
			if (IsValid(Highlightable))
			{
				IInv_Highlightable::Execute_HighLight(Highlightable);
			}
		}
		
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;
		
		if (IsValid(HUDWidget))
		{
			HUDWidget->ShowPickupMessage(ItemComponent->GetPickUpMessage());
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Started tracing a new actor."));
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
		{
			if (IsValid(Highlightable))
			{
				IInv_Highlightable::Execute_UnHighLight(Highlightable);
			}
		}
	}
	
	
}


