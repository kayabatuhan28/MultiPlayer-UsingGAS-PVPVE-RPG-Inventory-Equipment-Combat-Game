// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Items/Inv_BaseItemActor.h"
#include "Inv_ConsumableItemActor.generated.h"

class APlayerController;

UCLASS()
class TPGAS_API AInv_ConsumableItemActor : public AInv_BaseItemActor
{
	GENERATED_BODY()

public:
	AInv_ConsumableItemActor();

	UFUNCTION(BlueprintImplementableEvent)
	void OnConsumableUse(APlayerController* PC);

protected:
	
	virtual void BeginPlay() override;


};
