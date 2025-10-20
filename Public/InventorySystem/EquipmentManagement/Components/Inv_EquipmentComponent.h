// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Inv_EquipmentComponent.generated.h"

struct FInv_ItemManifest;
struct FInv_EquipmentFragment;
class AInv_EquipActor;
class UInv_InventoryItem;
class UInv_InventoryComponent;
class APlayerController;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TPGAS_API UInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh);
	void SetIsProxy(bool bProxy) { bIsProxy = bProxy; }
	void InitializeOwner(APlayerController* PlayerController);

	void ClearEquipWeaponData(const FGameplayTag& EquipmentTypeTag);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float WeaponEquippedMovementSpeed = 500.f;

private:

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnEquipped(UInv_InventoryItem* UnequippedItem);

	void InitInventoryComponent();
	void InitPlayerController();
	AInv_EquipActor* SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment, const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);

	UPROPERTY()
	TArray<TObjectPtr<AInv_EquipActor>> EquippedActors;

	AInv_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn);

	bool bIsProxy{false}; // Proxy Meshe özel (Inventoryde gozuken fake mesh icin) icin bazi duzenlemeler yapabilmek adina ekledik
	
};












