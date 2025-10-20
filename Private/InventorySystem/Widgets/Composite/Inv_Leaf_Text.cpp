// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widgets/Composite/Inv_Leaf_Text.h"

#include "Components/TextBlock.h"

void UInv_Leaf_Text::SetText(const FText& Text) const
{
	Text_LeafText->SetText(Text);
}

void UInv_Leaf_Text::SetTextColor(FSlateColor InColor) const
{
	Text_LeafText->SetColorAndOpacity(InColor);
}

void UInv_Leaf_Text::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo FontInfo = Text_LeafText->GetFont();
	FontInfo.Size = FontSize;
	
	Text_LeafText->SetFont(FontInfo);
}
