// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "GameFramework/TPGasPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetTPGasPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallBacksDependencies()
{
	// AbilityComptaki Status tagler her degistiginde bu kisim cagrilir ve widgetlara kendini updatelemesi için broadcast gönderilir.
	GetTPGasASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);

			// Bir skill globeyi sectigimiz zaman description text kismini setlemek icin
			FString Description;
			FString NextLevelDescription;
			GetTPGasASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FTPGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetTPGasASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetTPGasPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);

		// Bir skill globeyi sectigimiz zaman description text kismini setlemek icin
		FString Description;
		FString NextLevelDescription;
		GetTPGasASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});

}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// Senaryolar :
	// 1-) SpendPoint 0 olabilir bu durumda spendpoint button kapalı olması gerek, equip buttonuda alttaki caselere göre açık veya kapalı olabilir.
	// 2-) Seçilen skill button levelimiz yetmedigi icin locked olabilir bu durumda equip ve spendpoint button kapalı olması gerek
	// 3-) Seçilen skill button Eligible(Yani equiplenmemiş ama ögrenilebilir skillse) ise spendpoint ile skili açabiliriz ama açmadan equip yapamamamız gerek.
	// 4-) Seçilen skill öğrenilmiş ise spend point ile skili geliştirebiliriz yada equip diyip skili skill slota ekleyebiliriz yani 2 buttonda açık.

	// Bir skili secip equipe bastıktan sonra başka bir skili secersek  alttaki equip yapılabilecek slotu belirten animasyonun iptali icin
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FTpGasGameplayTags GameplayTags = FTpGasGameplayTags::Get();
	const int32 SpellPoints = GetTPGasPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetTPGasASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_SkillStatus_Locked;
	}
	else
	{
		AbilityStatus = GetTPGasASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);

	// Bir skill globeyi sectigimiz zaman description text kismini setlemek icin
	FString Description;
	FString NextLevelDescription;
	GetTPGasASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetTPGasASC())
	{
		GetTPGasASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	// Bir skili secip equipe bastıktan sonra tekrar basıp iptal edersek alttaki equip yapılabilecek slotu belirten animasyonun iptali icin
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	// Skill seciliyken tekrar basinca selecti kaldirma islemi
	SelectedAbility.Ability = FTpGasGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FTpGasGameplayTags::Get().Abilities_SkillStatus_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	// Bir skili secip equip buttona bastigimiz zaman bu skilin agresif veya pasif olmasina göre alttaki slotta animasyon oynatmak icin olusturduk.
	// Ör.Oyuncu Fireboltu secip equip derse altta agresif skill kısmını kapsayan bi bounding box animationu oynayıp bu skilin nereye koyulabilecegini göstermekte

	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	// Secilen slotu tutmak icin
	const FGameplayTag SelectedStatus = GetTPGasASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FTpGasGameplayTags::Get().Abilities_SkillStatus_Equipped))
	{
		SelectedSlot = GetTPGasASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;

	// Offensive bir ability secili ise bu ability nin passive slota koyulmamasi gerek. Secilen abilityType ile slotun abilityTypesi ayni olmali
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetTPGasASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SpellMenuWidgetController.cpp OnAbilityEquipped Girdi!!!"));
	
	bWaitingForEquipSelection = false;
	const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();

	// Clear the old slot
	FTPGasAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_SkillStatus_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if Previous Slot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill the new slot
	FTPGasAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);

	// Equip yapildigi anda skilin selectionunu kaldirmak icin ve equip buttonu disable yapıp description boxu clearlamak icin
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FTpGasGameplayTags GameplayTags = FTpGasGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
