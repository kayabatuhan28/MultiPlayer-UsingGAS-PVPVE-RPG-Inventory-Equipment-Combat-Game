// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Inv_Highlightable.generated.h"


UINTERFACE()
class UInv_Highlightable : public UInterface
{
	GENERATED_BODY()
};


class TPGAS_API IInv_Highlightable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void HighLight();

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void UnHighLight();
	
};
