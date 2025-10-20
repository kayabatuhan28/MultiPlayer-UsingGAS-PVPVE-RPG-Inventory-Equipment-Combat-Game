// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HoverItem.generated.h"

/**
 * HoverItem is the item that will appear and follow the mouse
 * when an inventory item on the grid has been clicked.
 */

class UInv_InventoryItem;
class UImage;
class UTextBlock;

UCLASS()
class TPGAS_API UInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 Count);

	// Getter
	FGameplayTag GetItemType() const;
	int32 GetStackCount() const { return StackCount; }
	bool IsStackable() const { return bIsStackable; }
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	UInv_InventoryItem* GetInventoryItem() const;
	
	// Setter
	void SetIsStackable(bool bStacks);
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	

	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
	
};
