// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widgets/HUD/Inv_HUDWidget.h"

#include "InventorySystem/InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventorySystem/InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventorySystem/Widgets/HUD/Inv_InfoMessage.h"

void UInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &ThisClass::OnNoRoom);
	}
}

void UInv_HUDWidget::OnNoRoom()
{
	if (!IsValid(InfoMessage)) return;
	InfoMessage->SetMessage(FText::FromString("No Room In Inventory."));
}
