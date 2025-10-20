// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Combat/CombatUtils.h"
#include "TPGasPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;
class UAbilityInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* Stat Value */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerXPChanged, int32 /* New XP Value */, int32 /* Gained XP Value */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSkillChanged, bool);


UCLASS()
class TPGAS_API ATPGasPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATPGasPlayerState();

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerXPChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;
	FOnWeaponSkillChanged OnWeaponSkillChanged;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPlayerLevel() const { return  Level; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetXP() const { return  XP; }

	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	UFUNCTION(BlueprintCallable)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintCallable)
	void AddToLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable)
	void AddToSpellPoints(int32 InPoints);

	UFUNCTION(BlueprintCallable)
	void SetXP(int32 InXP);

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 InLevel);

	UFUNCTION(Server, Reliable)
	void Server_WeaponChangeBroadcast(bool bIsUnEquip);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponChangeBroadcast(bool bIsUnEquip);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LevelUpEffect();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void XPGain(int32 GainedXP, int32 CurrentXP, int32 MaxXp);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LevelNotEnough(int32 CurrentPlayerLevel, int32 EquipmentRequiredLevel);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
};
