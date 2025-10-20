﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TPGAS_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void InitItemManifest(FInv_ItemManifest CopyOfManifest);
	FInv_ItemManifest GetItemManifest() const { return ItemManifest; }
	FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest; }
	FString GetPickUpMessage() const { return PickUpMessage; }
	void PickedUp();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedUp();
	

private:

	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickUpMessage;
	
	
};
