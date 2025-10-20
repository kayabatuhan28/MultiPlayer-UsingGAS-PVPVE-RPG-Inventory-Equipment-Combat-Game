// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPGasAbilitySystemLibrary.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/TPGasBaseCharacter.h"
#include "GameFramework/TPGasGameMode.h"
#include "GameFramework/TPGasPlayerState.h"
#include "Interface/ICombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/HUD/TPGasHUD.h"
#include "UI/WidgetController/TPGasWidgetController.h"

bool UTPGasAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                            FWidgetControllerParams& OutWCParams, ATPGasHUD*& OutTPGasHUD)
{
	if (APlayerController* PC =  UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutTPGasHUD = Cast<ATPGasHUD>(PC->GetHUD());
		if (OutTPGasHUD)
		{
			ATPGasPlayerState* PS = PC->GetPlayerState<ATPGasPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UTPGasAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATPGasHUD* TPGasHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TPGasHUD))
	{
		return TPGasHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UTPGasAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: WorldContextObject is null"));

		return nullptr;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: PlayerController not found"));

		return nullptr;
	}

	// UI sadece local client için kurulmalı
	if (!PC->IsLocalController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: Not a local controller, skipping"));
		
		return nullptr;
	}

	ATPGasHUD* TPGasHUD = Cast<ATPGasHUD>(PC->GetHUD());
	if (!TPGasHUD)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: HUD is not ATPGasHUD or not yet created"));

		return nullptr;
	}

	ATPGasPlayerState* PS = PC->GetPlayerState<ATPGasPlayerState>();
	if (!PS)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: PlayerState is null (probably not replicated yet)"));

		return nullptr;
	}

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!ASC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: AbilitySystemComponent is null"));
		
		return nullptr;
	}

	UAttributeSet* AS = PS->GetAttributeSet();
	if (!AS)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("GetSpellMenuWidgetController: AttributeSet is null"));
		
		return nullptr;
	}

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	return TPGasHUD->GetSpellMenuWidgetController(WidgetControllerParams);
}

UAbilityInfo* UTPGasAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	ATPGasPlayerState* PS = PC->GetPlayerState<ATPGasPlayerState>();
	if (!PS) return nullptr;

	return PS->AbilityInfo;
}

void UTPGasAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UTPGasAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	// Bu abilityler levele göre degismeyen temel abilityler : HitReact, ölme vs. gibi
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	// Startup abilities levele göre ai da ve playerda farklı olabilir o yüzden levele göre setledik.
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UICombatInterface>())
		{
			//FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassInfo* UTPGasAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	// GameMode only valid on the server, if it accessed from the client return nullptr
	const ATPGasGameMode* TPGasGameMode = Cast<ATPGasGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TPGasGameMode == nullptr) return nullptr;
	return TPGasGameMode->CharacterClassInfo;
}

void UTPGasAbilitySystemLibrary::HealTarget(AActor* HealedActor, TSubclassOf<UGameplayEffect> HealEffectClass, float HealAmount)
{
	if (HealedActor == nullptr) return;

	UAbilitySystemComponent* HealedTargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HealedActor);
	if (HealedTargetASC == nullptr) return;
	
	FGameplayEffectContextHandle ContextHandle = HealedTargetASC->MakeEffectContext();
	ContextHandle.AddSourceObject(HealedActor);

	FGameplayEffectSpecHandle DamageSpecHandle = HealedTargetASC->MakeOutgoingSpec(HealEffectClass, 1, ContextHandle);

	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag HealCallerTag = Tags.Heal;
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, HealCallerTag, HealAmount);

	HealedTargetASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), HealedTargetASC);
	
}

void UTPGasAbilitySystemLibrary::LifeSteal(ATPGasBaseCharacter* BaseCharacter, TSubclassOf<UGameplayEffect> HealEffectClass, float InComingDamage)
{
	if (BaseCharacter == nullptr) return;

	UAbilitySystemComponent* HealedTargetASC = BaseCharacter->GetAbilitySystemComponent();
	if (HealedTargetASC == nullptr) return;

	UTPGasAttributeSet* BaseCharacterAS = Cast<UTPGasAttributeSet>(BaseCharacter->GetAttributeSet());
	if (BaseCharacterAS == nullptr) return;

	float LifeStealValue = InComingDamage * BaseCharacterAS->GetLifeSteal() / 100.f;
	if (LifeStealValue < 1.f) return;

	FGameplayEffectContextHandle ContextHandle = HealedTargetASC->MakeEffectContext();
	ContextHandle.AddSourceObject(BaseCharacter);

	FGameplayEffectSpecHandle DamageSpecHandle = HealedTargetASC->MakeOutgoingSpec(HealEffectClass, 1, ContextHandle);

	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag HealCallerTag = Tags.Heal;
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, HealCallerTag, LifeStealValue);

	HealedTargetASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), HealedTargetASC);
}

EHitDirection UTPGasAbilitySystemLibrary::CalculateHitDirection(AActor* DamageCauser, AActor* DamagedActor)
{
	float DotProduct = DamagedActor->GetDotProductTo(DamageCauser);
	if (DotProduct <= 0.f)
	{
		return EHitDirection::Backward;
	}

	return EHitDirection::Forward;
}

void UTPGasAbilitySystemLibrary::SetReplicatedRotateToTargetActor(AActor* InRotatingActor, AActor* InTargetActor)
{
	if (ATPGasBaseCharacter* RotatingCharacter = Cast<ATPGasBaseCharacter>(InRotatingActor))
	{
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(InRotatingActor->GetActorLocation(), InTargetActor->GetActorLocation());
		Rotator.Pitch = 0.f;
		Rotator.Roll = 0.f;
		RotatingCharacter->SetActorRotationReplicated(Rotator);
	}
}

int32 UTPGasAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}






