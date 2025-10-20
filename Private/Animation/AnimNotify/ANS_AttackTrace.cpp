// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/ANS_AttackTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TPGasGameplayTags.h"
#include "Character/TPGasCharacter.h"
#include "Combat/Component/TPGasCombatComp.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor_WeaponBase.h"


void UANS_AttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ATPGasCharacter* PlayerRef = Cast<ATPGasCharacter>(MeshComp->GetOwner());
	if (!IsValid(PlayerRef)) return;

	if (PlayerRef->CombatComp->EquippedWeapon == nullptr) return;

	if (PlayerRef->GetAbilitySystemComponent() == nullptr) return;

	// Trace Data
	UStaticMeshComponent* WeaponMesh =  PlayerRef->CombatComp->EquippedWeapon->WeaponMesh;
	FHitResult HitResult;
	FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart"); // Get the center of the meshFVector 
	FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	FRotator TraceRot = WeaponMesh->GetComponentRotation(); // Get the rotation of the meshFQuat
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(PlayerRef);
	for (AActor* SummonedRef : PlayerRef->SummonedActors)
	{
		IgnoredActors.Add(SummonedRef);
	}
	
	// Trace
	UKismetSystemLibrary::BoxTraceSingle(
		PlayerRef->GetWorld(),
		TraceStart,
		TraceEnd,
		HalfSize,
		TraceRot,
		Query,
		true,
		IgnoredActors,
		DrawDebugType,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.f
		);

	if (HitResult.bBlockingHit == false) return;

	ATPGasBaseCharacter* HittedCharacter = Cast<ATPGasBaseCharacter>(HitResult.GetActor());
	if (IsValid(HittedCharacter))
	{
		UAbilitySystemComponent* HittedActorASC = HittedCharacter->GetAbilitySystemComponent();
		if (HittedActorASC == nullptr) return;
		
		const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
		FGameplayTag PvpDisableTag = Tags.Effects_PvpDisabled;
		FGameplayTag SummonIsFriendTag = Tags.Effects_Ai_SummonIsFriend;
		
		bool Cond1 = HittedActorASC->HasMatchingGameplayTag(PvpDisableTag) == false;
		bool Cond2 = HittedActors.Contains(HitResult.GetActor()) == false;
		bool bIsSummonFriend = HittedActorASC->HasMatchingGameplayTag(SummonIsFriendTag);
		
		if (Cond1 && Cond2)
		{
			if (bIsSummonFriend == false && !HittedActors.Contains(HitResult.GetActor()))
			{
				HittedActors.AddUnique(HitResult.GetActor());
				FGameplayEventData Payload;
				Payload.Target = HitResult.GetActor();
				const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Event.Combat.AttackTrace");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerRef, EventTag, Payload);
			}
		}
	}
	else // BaseCharacterden tureyen bir sey hit almadiysa gather icin tasa felan vurduysak misal bu kisimda handle et
	{
		
	}
	
	
}

void UANS_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HittedActors.Empty();
}
