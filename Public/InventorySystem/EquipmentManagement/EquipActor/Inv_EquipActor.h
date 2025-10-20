// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Inv_EquipActor.generated.h"

UENUM()
enum class EEquipArmorType : uint8
{
	None = 0,
	Head = 1,
	Body = 2,
	Legs = 3,
	Arm = 4,
	Foot = 5,
	Glove = 6
};

UCLASS()
class TPGAS_API AInv_EquipActor : public AActor
{
	GENERATED_BODY()

public:
	
	AInv_EquipActor();
	
	virtual void BeginPlay() override;
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(FGameplayTag Type) { EquipmentType = Type; }
	virtual void SetAttachmentData();
	

private:

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentType;

	



};
