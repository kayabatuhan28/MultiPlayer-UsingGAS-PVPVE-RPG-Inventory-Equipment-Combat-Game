﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/Types/Inv_GridTypes.h"
#include "Inv_InventoryBase.generated.h"

class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_HoverItem;

UCLASS()
class TPGAS_API UInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const { return FInv_SlotAvailabilityResult(); }
	virtual void OnItemHovered(UInv_InventoryItem* Item) { }
	virtual void OnItemUnhovered() { }
	virtual bool HasHoverItem() const { return false; }
	virtual UInv_HoverItem* GetHoverItem() const { return nullptr; }
	virtual float GetTileSize() const { return 0.f; }
	
};
