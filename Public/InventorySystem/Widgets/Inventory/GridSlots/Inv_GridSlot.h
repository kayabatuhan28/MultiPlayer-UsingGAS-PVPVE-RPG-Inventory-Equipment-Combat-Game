﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UInv_ItemPopUp;
class UImage;
class UInv_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

UCLASS()
class TPGAS_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Getter
	int32 GetTileIndex() const {return TileIndex; }
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const {return InventoryItem; }
	int32 GetStackCount() const {return StackCount; }
	int32 GetIndex() const { return TileIndex; }
	int32 GetUpperLeftIndex() const {return UpperLeftIndex; }
	EInv_GridSlotState GetGridSlotState() const {return GridSlotState; }
	bool IsAvailable() const { return bAvailable; }
	UInv_ItemPopUp* GetItemPopUp() const;
	
	// Setter
	void SetTileIndex(int32 Index) {TileIndex = Index; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	void SetStackCount(int32 Count) {StackCount = Count; }
	void SetIndex(int32 Index) {TileIndex = Index; }
	void SetUpperLeftIndex(int32 Index) {UpperLeftIndex = Index; }
	void SetAvailable(bool bIsAvailable) {bAvailable = bIsAvailable; }
	void SetItemPopUp(UInv_ItemPopUp* PopUp);	
	

	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;

	
private:

	int32 StackCount{0};
	bool bAvailable{true};
	int32 TileIndex{INDEX_NONE};
	int32 UpperLeftIndex{INDEX_NONE};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	TWeakObjectPtr<UInv_ItemPopUp> ItemPopUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;
	
	EInv_GridSlotState GridSlotState;

	UFUNCTION()
	void OnItemPopUpDestruct(UUserWidget* Menu);
	
	
};
