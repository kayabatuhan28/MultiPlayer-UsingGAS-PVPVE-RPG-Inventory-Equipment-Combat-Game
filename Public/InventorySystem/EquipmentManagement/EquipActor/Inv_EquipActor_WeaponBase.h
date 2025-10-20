// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_EquipActor.h"
#include "Combat/CombatUtils.h"
#include "Inv_EquipActor_WeaponBase.generated.h"

class UGameplayEffect;

UCLASS()
class TPGAS_API AInv_EquipActor_WeaponBase : public AInv_EquipActor
{
	GENERATED_BODY()

public:
	AInv_EquipActor_WeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCombatData WeaponCombatData{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh{};

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MainRootComponent{};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_WeaponEquipped{};

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RemovedEquippedGameplayTag{};

protected:
	
	virtual void BeginPlay() override;
	virtual void SetAttachmentData() override;
	virtual void Destroyed() override;


};
