// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_EquipActor.h"
#include "Inv_EquipActor_ArmorBase.generated.h"

UCLASS()
class TPGAS_API AInv_EquipActor_ArmorBase : public AInv_EquipActor
{
	GENERATED_BODY()

public:
	AInv_EquipActor_ArmorBase();

	virtual void SetAttachmentData() override;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	bool bSetLeaderPoseData = false;
	
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, meta=(EditCondition="bSetLeaderPoseData"))
	USkinnedAsset* NewSkinnedAsset{};
	
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, meta=(EditCondition="bSetLeaderPoseData"))
	USkeletalMesh* NewSkeletalMesh{};

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, meta=(EditCondition="bSetLeaderPoseData"))
	EEquipArmorType EquipActorType{};

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


};
