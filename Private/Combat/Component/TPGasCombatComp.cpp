// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Component/TPGasCombatComp.h"

#include "Character/TPGasCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_WeaponBase.h"
#include "Net/UnrealNetwork.h"


UTPGasCombatComp::UTPGasCombatComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UTPGasCombatComp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPGasCombatComp, EquippedWeapon);	
}

void UTPGasCombatComp::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<ATPGasCharacter>(GetOwner());
	
}

void UTPGasCombatComp::SetNewWeapon(AInv_EquipActor_WeaponBase* NewWeapon)
{
	EquippedWeapon = NewWeapon;
}

void UTPGasCombatComp::Server_SetLookingDirection_Implementation(bool bIsLookingDirection)
{
	if (!IsValid(OwnerPlayer)) return;
	Multicast_SetLookingDirection(bIsLookingDirection);
}

void UTPGasCombatComp::Multicast_SetLookingDirection_Implementation(bool bIsLookingDirection)
{
	if (bIsLookingDirection)
	{
		OwnerPlayer->bUseControllerRotationYaw = true;
		OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		OwnerPlayer->bUseControllerRotationYaw = false;
		OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

EWeaponType UTPGasCombatComp::GetEquippedWeaponBlendspace()
{
	if (IsValid(EquippedWeapon))
	{
		return EquippedWeapon->WeaponCombatData.CombatAnimationData.WeaponType;
	}
	
	return EWeaponType::UnEquipped;
}

UAnimMontage* UTPGasCombatComp::GetLightAttackMontage()
{
	if (!IsValid(EquippedWeapon)) return nullptr;

	if (bLightAttackComboSaved)
	{
		if (LightAttackIndex + 1 >= EquippedWeapon->WeaponCombatData.CombatAnimationData.LightAttackMontages.Num())
		{
			LightAttackIndex = 0;
			return EquippedWeapon->WeaponCombatData.CombatAnimationData.LightAttackMontages[0];
		}
		LightAttackIndex++;
		return EquippedWeapon->WeaponCombatData.CombatAnimationData.LightAttackMontages[LightAttackIndex];
	}

	LightAttackIndex = 0;
	return EquippedWeapon->WeaponCombatData.CombatAnimationData.LightAttackMontages[0];
}

UAnimMontage* UTPGasCombatComp::GetAfterDodge_ForwardAttackMontage()
{
	if (!IsValid(EquippedWeapon)) return nullptr;

	return EquippedWeapon->WeaponCombatData.CombatAnimationData.AfterForwardDodgeAttack;
}

UAnimMontage* UTPGasCombatComp::GetAfterDodge_BackwardAttackMontage()
{
	if (!IsValid(EquippedWeapon)) return nullptr;

	return EquippedWeapon->WeaponCombatData.CombatAnimationData.AfterBackwardDodgeAttack;
}

UAnimMontage* UTPGasCombatComp::GetHeavyAttackMontage()
{
	if (!IsValid(EquippedWeapon)) return nullptr;

	if (bHeavyAttackComboSaved)
	{
		if (HeavyAttackIndex + 1 >= EquippedWeapon->WeaponCombatData.CombatAnimationData.HeavyAttackMontages.Num())
		{
			HeavyAttackIndex = 0;
			return EquippedWeapon->WeaponCombatData.CombatAnimationData.HeavyAttackMontages[0];
		}
		HeavyAttackIndex++;
		return EquippedWeapon->WeaponCombatData.CombatAnimationData.HeavyAttackMontages[HeavyAttackIndex];
	}

	HeavyAttackIndex = 0;
	return EquippedWeapon->WeaponCombatData.CombatAnimationData.HeavyAttackMontages[0];
}

bool UTPGasCombatComp::IsComboEndAttack()
{
	if (!IsValid(EquippedWeapon)) return false;

	if (LightAttackIndex == EquippedWeapon->WeaponCombatData.CombatAnimationData.LightAttackMontages.Num()-1)
	{
		return true;
	}
	return false;
}

void UTPGasCombatComp::EnableLightAttackComboPeriod(float TimerDelay)
{
	if (LightAttackSavedTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(LightAttackSavedTimerHandle);
	}
	
	bLightAttackComboSaved = true;
	GetWorld()->GetTimerManager().SetTimer(LightAttackSavedTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		bLightAttackComboSaved = false;
	}), TimerDelay, false);
}

void UTPGasCombatComp::EnableHeavyAttackComboPeriod(float TimerDelay)
{
	if (HeavyAttackSavedTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HeavyAttackSavedTimerHandle);
	}
	
	bHeavyAttackComboSaved = true;
	GetWorld()->GetTimerManager().SetTimer(HeavyAttackSavedTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		bHeavyAttackComboSaved = false;
	}), TimerDelay, false);
}







