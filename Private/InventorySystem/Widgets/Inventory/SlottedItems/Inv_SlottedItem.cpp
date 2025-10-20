// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "InventorySystem/Items/Inv_InventoryItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/InventoryManagement/Utils/Inv_InventoryStatics.h"

FReply UInv_SlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, MouseEvent);
	return FReply::Handled();
}

void UInv_SlottedItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInv_InventoryStatics::ItemHovered(GetOwningPlayer(), InventoryItem.Get());
}

void UInv_SlottedItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());
}

void UInv_SlottedItem::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
}

void UInv_SlottedItem::SetImageBrush(const FSlateBrush Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UInv_SlottedItem::UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
