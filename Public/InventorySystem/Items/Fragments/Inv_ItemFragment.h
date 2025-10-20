#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemFragment.generated.h"

class UGameplayEffect;
class APlayerController;

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	FInv_ItemFragment() {}
	//FInv_ItemFragment(const FInv_ItemFragment& ) = default;
	//FInv_ItemFragment& operator = (const FInv_ItemFragment& ) = default;
	//FInv_ItemFragment(const FInv_ItemFragment&& ) = default;
	//FInv_ItemFragment& operator = (const FInv_ItemFragment&& ) = default;
	virtual ~FInv_ItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	virtual void Manifest() {}

private:

	// Sadece fragmentTagsleri select edebilmek icin en sagdaki meta tanimlamasini ekledik.
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
	
};

/*
 *	Item fragment specifically for assimilation into a widget
 */
class UInv_CompositeBase;
USTRUCT(BlueprintType)
struct FInv_InventoryItemFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UInv_CompositeBase* Composite) const;

protected:

	bool MatchesWidgetTag(const UInv_CompositeBase* Composite) const;
	
};


USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(FIntPoint Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1, 1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{0.f};
	
};


USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return Icon; }
	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions{44.f, 44.f};
	
};

USTRUCT(BlueprintType)
struct FInv_TextFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	FText GetText() const { return FragmentText; }
	void SetText(const FText& Text) { FragmentText = Text; }
	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateColor TextColor = FSlateColor(FLinearColor::White);

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;

	
	
};

USTRUCT(BlueprintType)
struct FInv_LabeledNumberFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UInv_CompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
	

	// When manifesting for the first time, this fragment will randomize. However, one equipped 
	// and dropped an item should retain the same value, so randomization should not occur
	bool bRandomizeOnManifest{true};

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};
	
};

USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }

	void SetStackCount(int32 Count) { StackCount = Count; }

private:

	// Max Stack 10 varsayalim, 20 stackli bir item alirsak 10 10 iki ayri slota eklenir MaxStack 20 olsaydi tek slota eklenirdi onun icin
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	// Bir itemin kac tane stack verecegini temsil eder.
	UPROPERTY(editAnywhere, Category = "Inventory")
	int32 StackCount{1};
	
};

// Consume Framents
USTRUCT(BlueprintType)
struct FInv_ConsumeModifier : public FInv_LabeledNumberFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MagnitudeCallBackTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> MagnitudeGameplayEffect;

	void ApplyMagnitudeGameplayEffect(APlayerController* PC, float ConsumeValue);

	virtual void OnConsume(APlayerController* PC) {}
};

class AInv_ConsumableItemActor;
USTRUCT(BlueprintType)
struct FInv_ConsumableFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<AInv_ConsumableItemActor> ConsumableItemActorClass;
	
	virtual void OnConsume(APlayerController* PC);
	virtual void Assimilate(UInv_CompositeBase* Composite) const override;
	virtual void Manifest() override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ConsumeModifier>> ConsumeModifiers;

	
};

USTRUCT(BlueprintType)
struct FInv_HealthPotionFragment : public FInv_ConsumeModifier
{
	GENERATED_BODY()
	
	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FInv_ManaPotionFragment : public FInv_ConsumeModifier
{
	GENERATED_BODY()
	
	virtual void OnConsume(APlayerController* PC) override;
};

// Equipment
// --------------------------------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FInv_EquipModifier : public FInv_LabeledNumberFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MagnitudeCallBackTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> MagnitudeGameplayEffect;

	void ApplyMagnitudeGameplayEffect(APlayerController* PC, bool bIsAdded);

	 void OnEquip(APlayerController* PC);
	 void OnUnEquip(APlayerController* PC);
};

class AInv_EquipActor;
USTRUCT(BlueprintType)
struct FInv_EquipmentFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	bool bEquipped{false};
	void OnEquip(APlayerController* PC);
	void OnUnEquip(APlayerController* PC);
	virtual void Assimilate(UInv_CompositeBase* Composite) const override;
	virtual void Manifest() override;

	AInv_EquipActor* SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachedActor() const;
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquippedActor(AInv_EquipActor* EquipActor, APlayerController* PC);

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TInstancedStruct<FInv_EquipModifier>> EquipModifiers;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AInv_EquipActor> EquipActorClass = nullptr;

	TWeakObjectPtr<AInv_EquipActor> EquippedActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName SocketAttachPoint{NAME_None};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
};




