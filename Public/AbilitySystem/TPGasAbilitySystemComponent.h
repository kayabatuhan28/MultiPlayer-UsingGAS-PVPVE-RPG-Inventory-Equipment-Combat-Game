// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPGasAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* Ability Tag */, const FGameplayTag& /* Status Tag */, int32 /* Ability Level */);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /* Ability*/, const FGameplayTag& /* Status*/, const FGameplayTag& /* Slot*/, const FGameplayTag& /* PrevSlot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /* Ability*/ )
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);

UCLASS()
class TPGAS_API UTPGasAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	// Bu fonksiyonu InitAbilityActorInfo cagrildiktan sonra cagirdik. Gastaki bazi özel delegateleri playerda ve enemylerde bindlamak icin olusturduk.
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	// Abilityi karaktere verdigimiz zaman bu delegate ile Skillerin oldugu widgeta bunun bilgisini göndermek icin ekledik.
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;
	
	// Karakterlere baslangic abilitysi vermek icin olusturduk. Karakter classinda bunu cagiririz.
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> & StartupAbilities, int32 AbilityStartLevel);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> & StartupPassiveAbilities);

	// Bu booleani overlaywidgetcontrollerda abilityler setlenmeden abilityleri skill widgeta bindlama isleminden kacinmak icin ekledik. Abilitylerin verildiginden
	// net emin olmadan bindlamamak gerektigi icin.
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);
	bool SlotIsEmpty(const FGameplayTag& Slot);
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	// Passive aktif edince replike broadcast islemi yaparak vfx attachi icin
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatues(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);

protected:

	// Bu kismi input taglere göre skillbara skill ekledigimiz zaman serverde skill slota skill setlenip cliente setlenmedigi zaman farkettik. AddCharacterAbilities
	// fonksiyonundaki delegate sadece serverde handle edilmekte Clientlerde broadcast edilmemekteydi. Kütüphaneden gelen bu fonksiyon karakter ability aktif ettiği zaman
	// otomatik diger clientlerede replike edildigi icin broadcast islemini burayada ekledik.
	virtual void OnRep_ActivateAbilities() override;

	// Constructtaki OnGameplayEffectAppliedDelegateToSelf'ye ctrl ile gidersek sadece serverde oldugunu görürüz. Clientlerede replike etmek icin rpc client ekledik.
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};



