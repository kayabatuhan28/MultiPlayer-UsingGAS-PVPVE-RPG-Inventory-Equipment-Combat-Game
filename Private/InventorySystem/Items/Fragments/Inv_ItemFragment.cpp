#include "InventorySystem/Items/Fragments/Inv_ItemFragment.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Character/TPGasCharacter.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "InventorySystem/Widgets/Composite/Inv_CompositeBase.h"
#include "InventorySystem/Widgets/Composite/Inv_Leaf_Image.h"
#include "InventorySystem/Widgets/Composite/Inv_Leaf_LabeledValue.h"
#include "InventorySystem/Widgets/Composite/Inv_Leaf_Text.h"

void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Text* LeafText = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(LeafText)) return;

	LeafText->SetText(FragmentText);
	LeafText->SetTextColor(TextColor);
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;
	
	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
	
}

void FInv_LabeledNumberFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}




// Consumable Fragments Struct Functions
// ----------------------------------------------------------------------------------------------------------------------------------------------
void FInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable<>();
		ModRef.OnConsume(PC);
	}

	/*
	if (IsValid(ConsumableItemActorClass))
	{
		AInv_ConsumableItemActor* SelectedConsumables = Cast<AInv_ConsumableItemActor>(ConsumableItemActorClass.GetDefaultObject());
		if (SelectedConsumables)
		{
			SelectedConsumables->OnConsumableUse(PC);
		}
	}
	*/
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable<>();
		ModRef.Manifest();
	}
	
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	ApplyMagnitudeGameplayEffect(PC, GetValue());
	// TODO : Add if health potion extra do something
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	ApplyMagnitudeGameplayEffect(PC, GetValue());
	// TODO : Add if mana potion extra do something
}

void FInv_ConsumeModifier::ApplyMagnitudeGameplayEffect(APlayerController* PC, float ConsumeValue)
{
	if (!IsValid(PC)) return;

	ATPGasCharacter* PlayerCharacter = Cast<ATPGasCharacter>(PC->GetPawn());
	if (!IsValid(PlayerCharacter)) return;

	UAbilitySystemComponent* PlayerAsc = PlayerCharacter->GetAbilitySystemComponent();
	if (PlayerAsc == nullptr) return;

	FGameplayEffectContextHandle ContextHandle;
	
	FGameplayEffectSpecHandle DamageSpecHandle = PlayerAsc->MakeOutgoingSpec(MagnitudeGameplayEffect, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, MagnitudeCallBackTag, ConsumeValue);

	PlayerAsc->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

// Consumable Fragments Struct Functions
// ----------------------------------------------------------------------------------------------------------------------------------------------


// Equipment Fragment Struct Functions
// ----------------------------------------------------------------------------------------------------------------------------------------------
void FInv_EquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;

	bEquipped = true;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC);
	}
}

void FInv_EquipmentFragment::OnUnEquip(APlayerController* PC)
{
	if (!bEquipped) return;

	bEquipped = false;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnEquip(PC);
	}
}

void FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_EquipmentFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable<>();
		ModRef.Manifest();
	}
}

AInv_EquipActor* FInv_EquipmentFragment::SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(EquipActorClass) || !IsValid(AttachMesh)) return nullptr;

	AInv_EquipActor* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<AInv_EquipActor>(EquipActorClass);
	SpawnedActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint);
	return SpawnedActor;
}

void FInv_EquipmentFragment::DestroyAttachedActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}

void FInv_EquipmentFragment::SetEquippedActor(AInv_EquipActor* EquipActor, APlayerController* PC)
{
	EquippedActor = EquipActor;
}

// Equipment Fragment Struct Functions
// ----------------------------------------------------------------------------------------------------------------------------------------------


// Equip Modifiers
// ----------------------------------------------------------------------------------------------------------------------------------------------
void FInv_EquipModifier::ApplyMagnitudeGameplayEffect(APlayerController* PC, bool bIsAdded)
{
	const float Amount = bIsAdded ? GetValue() : -GetValue();

	if (!IsValid(PC)) return;

	ATPGasCharacter* PlayerCharacter = Cast<ATPGasCharacter>(PC->GetPawn());
	if (!IsValid(PlayerCharacter)) return;

	UAbilitySystemComponent* PlayerAsc = PlayerCharacter->GetAbilitySystemComponent();
	if (PlayerAsc == nullptr) return;

	FGameplayEffectContextHandle ContextHandle;
	
	FGameplayEffectSpecHandle DamageSpecHandle = PlayerAsc->MakeOutgoingSpec(MagnitudeGameplayEffect, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, MagnitudeCallBackTag, Amount);

	PlayerAsc->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

void FInv_EquipModifier::OnEquip(APlayerController* PC)
{
	ApplyMagnitudeGameplayEffect(PC, true);
}

void FInv_EquipModifier::OnUnEquip(APlayerController* PC)
{
	ApplyMagnitudeGameplayEffect(PC, false);
}


