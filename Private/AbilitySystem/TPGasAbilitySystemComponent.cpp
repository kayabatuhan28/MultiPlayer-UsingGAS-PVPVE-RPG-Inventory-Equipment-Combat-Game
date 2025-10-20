// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPGasAbilitySystemComponent.h"

#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/TPGasGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interface/PlayerInterface.h"

void UTPGasAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTPGasAbilitySystemComponent::ClientEffectApplied);
}

void UTPGasAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities, int32 AbilityStartLevel)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, AbilityStartLevel);

		// Abilitylere dynamic olarak input tag ekler. Dynamic oldugu icin runtimede eklenip degistirilip silinebilir
		if (const UTPGasGameplayAbility* AuraAbility = Cast<UTPGasGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec); // Sadece Ability verir, baslangicta direk aktif olmaz
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UTPGasAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UTPGasAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) 
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive()) 
			{
				// Wait input press fonksiyonunun calismasi icin ekledik, Held sürekli cagrildigi icin bu eventin 1 kere cagrilmasi yeterli olacagi icin
				// Ability InputTagPressed kisminda bu islemi yaptik. AuraPlayerController.cpp de InputPresste bu fonksiyonu cagirdik.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UTPGasAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// üstte karaktere ability eklerken ability speclere dynamic tagte ekledik.
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) 
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive()) // Ability aktif degilse 
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UTPGasAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive()) 
		{
			// Dont work 5.5+ -- Deprecated
			//AbilitySpecInputReleased(AbilitySpec);
			//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
			FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, OriginalPredictionKey);
		}
	}
}

void UTPGasAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			//UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %s"), __FUNCTION__); // Fonksiyon ismini yazar
		}
	}
}

FGameplayTag UTPGasAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.SkillStatus"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UTPGasAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UTPGasAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UTPGasAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	
	return true;
}

bool UTPGasAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(Slot);
}

bool UTPGasAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UTPGasAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UTPGasAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	const UAbilityInfo* AbilityInfo = UTPGasAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FTPGasAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return  AbilityType.MatchesTagExact(FTpGasGameplayTags::Get().Abilities_Type_Passive);
}

void UTPGasAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
}

void UTPGasAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

void UTPGasAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	/*
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
	*/
}

void UTPGasAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	/*
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	// Pasif sürekli aktif olan GA_ListenForEvent'e bilgiyi gönderir. Orada Attributes. uzantili herhangi bir tag event beklenmekte
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
	*/
}

void UTPGasAbilitySystemComponent::UpdateAbilityStatues(int32 Level)
{
	UAbilityInfo* AbilityInfo = UTPGasAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FTPGasAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			
			// Level ve gerekli şartlar tutunca ilgili ability nin statusunu ögrenilebilire ceker.
			AbilitySpec.DynamicAbilityTags.AddTag(FTpGasGameplayTags::Get().Abilities_SkillStatus_Eligible);
			GiveAbility(AbilitySpec);
			
			// if you ever want to force an ability spec to replicate now instead of waiting till the next update you call mark ability spec and we're going to do
			// that as it's important that our clients know right away if this change has occured and if we do that, than we can start thinking about making a broadcast
			// to our widget controller, our spell menu widget controller, so that it can update itself.
			MarkAbilitySpecDirty(AbilitySpec);
			
			ClientUpdateAbilityStatus(Info.AbilityTag, FTpGasGameplayTags::Get().Abilities_SkillStatus_Eligible, 1);
		}
	}
}

void UTPGasAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag)) // null döndürebilir
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);	
		}
		
		// Bu ife girdigi icin ability locket degildir, unlocked,eliible veya equipped olabilir bu senaryolari check etmek gerek
		const FTpGasGameplayTags GameplayTags = FTpGasGameplayTags::Get();

		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		
		
		if (Status.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Eligible))
		{
			// Eligible ise (Koyu giri ögrenilebilir) skill ögrenildigi zaman rengi acılıp unlocked hale gelir.
			// Eligibleden unlockeda gecer ability ve eligible tagi removelayıp yeni tagi setleriz.
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_SkillStatus_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_SkillStatus_Unlocked);
			Status = GameplayTags.Abilities_SkillStatus_Unlocked;
			
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_SkillStatus_Unlocked))
		{
			// unlocked ve equipped ise 2 sindede abilityi level atlatacagımız icin aynı islemden gecerler. Ability statusu degismez
			AbilitySpec->Level += 1;
		}

		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
		
	}
}

void UTPGasAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);
		
		// Skill Locked ise veya ögrenmeye acik ama ögrenilmemis ise equip yapamayiz.Hali hazirda equiplenmis yada ögrenilmis skill olmali
		const bool bStatusValid = Status == GameplayTags.Abilities_SkillStatus_Equipped || Status == GameplayTags.Abilities_SkillStatus_Unlocked;
		if (bStatusValid)
		{
			
			// Handle activation/deactivation passive abilities
			
			if (!SlotIsEmpty(Slot)) // There is an ability in this slot already. Deactivate and clear its slot.
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if (SpecWithSlot)
				{
					// Equiplemeye calistigimiz ability ile slottaki abilitynin tagi ayni ise yani ayni ability ise direk returnleyebiliriz bir sey yapmayiz.
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_SkillStatus_Equipped, Slot, PrevSlot);
						return;
					}

					// Pasif ability equiplenmekte. Slottaki mevcut pasif abilityi deactivate et
					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}

					ClearSlot(SpecWithSlot);
				}
			}

			// Ability doesn't yet have a slot (it's not active)
			if (!AbilityHasAnySlot(*AbilitySpec))
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			AssignSlotToAbility(*AbilitySpec, Slot);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_SkillStatus_Equipped, Slot, PrevSlot);
	}
}

void UTPGasAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UTPGasAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UTPGasGameplayAbility* AuraAbility = Cast<UTPGasGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}

	// Activatable ability bulamadigimiz zaman Locked Ability buldugumuzu acmak için gereken leveli vs. descriptiona setleyecek düzenleme
	UAbilityInfo* AbilityInfo = UTPGasAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	if (!AbilityInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilityInfo is NULL for Actor [%s]"), *GetNameSafe(GetAvatarActor()));
		OutDescription = FString("Missing AbilityInfo DataAsset!");
		OutNextLevelDescription = FString();
		return false;
	}

	// Level almamiz halinde hicbir skill bari secmedigimiz halde descriptiona text geliyordu onu fixlemek icin bu if checki ekledik.
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FTpGasGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UTPGasGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	
	OutNextLevelDescription = FString();
	return false;
}

void UTPGasAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	//MarkAbilitySpecDirty(*Spec); // Replike olmasi icin eklemistik basta ama sonradan kendimiz replike edince kaldirdik.
}

void UTPGasAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UTPGasAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

FGameplayTag UTPGasAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UTPGasAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	
	return FGameplayTag();
}



FGameplayAbilitySpec* UTPGasAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}


void UTPGasAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	// Her ability aktif ettigimizde calismasini engellemek icin ekledik. 
	if (!bStartupAbilitiesGiven)
	{
		AbilitiesGivenDelegate.Broadcast();
		bStartupAbilitiesGiven = true;
	}
}

void UTPGasAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UTPGasAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                      const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	// Effectle gelen gameplayTagContaineri broadcast ederiz. OverlayWidgetController.cpp de BindCallBacksDependencies bu delegate baglamasini yaptik.
	EffectAssetTags.Broadcast(TagContainer);
}
