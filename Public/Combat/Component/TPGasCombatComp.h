// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatUtils.h"
#include "TPGasCombatComp.generated.h"


class ATPGasCharacter;
class AInv_EquipActor_WeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPGAS_API UTPGasCombatComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPGasCombatComp();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
	AInv_EquipActor_WeaponBase* EquippedWeapon{};
	
	UPROPERTY()
	ATPGasCharacter* OwnerPlayer;

	void SetNewWeapon(AInv_EquipActor_WeaponBase* NewWeapon);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetLookingDirection(bool bIsLookingDirection);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetLookingDirection(bool bIsLookingDirection);

	FTimerHandle LightAttackSavedTimerHandle;
	FTimerHandle HeavyAttackSavedTimerHandle;
	
	UPROPERTY(BlueprintReadWrite, Category = "CombatData")
	int32 LightAttackIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CombatData")
	int32 HeavyAttackIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CombatData")
	bool bLightAttackComboSaved{};

	UPROPERTY(BlueprintReadWrite, Category = "CombatData")
	bool bHeavyAttackComboSaved{};

	// Getters
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EWeaponType GetEquippedWeaponBlendspace();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetLightAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAfterDodge_ForwardAttackMontage();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAfterDodge_BackwardAttackMontage();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetHeavyAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	bool IsComboEndAttack();

	

	// Setter
	UFUNCTION(BlueprintCallable)
	void EnableLightAttackComboPeriod(float TimerDelay);

	UFUNCTION(BlueprintCallable)
	void EnableHeavyAttackComboPeriod(float TimerDelay);
	

protected:
	virtual void BeginPlay() override;

	


};
