// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TPGasPlayerController.generated.h"

class UInv_InventoryComponent;
class UInv_HUDWidget;
class UInputMappingContext; // Build.cs te "EnhancedInput" ekle InputMappingContexti kullanabilmek için
class UInputAction;
class UTPGasInputConfig;
struct FInputActionValue;
class UTPGasAbilitySystemComponent;
class UTPGasUserWidget;
class ATargetArea;
class AInv_ChestActor;

UCLASS()
class TPGAS_API ATPGasPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction{};

public:
	ATPGasPlayerController();
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement Value")
	float ActionValueX{};

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement Value")
	float ActionValueY{};

	UPROPERTY(BlueprintReadWrite, Category = "AData")
	AInv_ChestActor* InteractableChest{};

	UFUNCTION(Server,Reliable)
	void Server_RequestOpenChest(AInv_ChestActor* ChestRef);


protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void MoveCompleted();
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_SetInputActionValues(float InActionValueX, float InActionValueY);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetInputActionValues(float InActionValueX, float InActionValueY);

private:
	
	void PrimaryInteract();
	void CreateHudWidget();
	void TraceForItem();
	void ToggleStatMenu();
	void ToggleSkillMenu();
	
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTPGasInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UTPGasAbilitySystemComponent> TPGasAbilitySystemComponent;

	UTPGasAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> OpenSkillMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> OpenStatMenuAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HudWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	// Open Stat Menu Class
	bool bIsOpenStatMenuOpen = false;
	bool bIsSkillMenuOpen = false;
	// Open Stat Menu Class

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	
	
};




