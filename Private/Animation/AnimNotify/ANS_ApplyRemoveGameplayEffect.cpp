// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/ANS_ApplyRemoveGameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "Character/TPGasBaseCharacter.h"

void UANS_ApplyRemoveGameplayEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ATPGasBaseCharacter* OwnerCharacter = Cast<ATPGasBaseCharacter>(MeshComp->GetOwner()))
	{
		if (OwnerCharacter->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle ContextHandle;
			ContextHandle.AddSourceObject(OwnerCharacter);
			OwnerCharacter->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToSelf(AppliedGameplayEffectClass, 1, ContextHandle);
		}
	}
}

void UANS_ApplyRemoveGameplayEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ATPGasBaseCharacter* OwnerCharacter = Cast<ATPGasBaseCharacter>(MeshComp->GetOwner()))
	{
		if (OwnerCharacter->GetAbilitySystemComponent())
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(RemovedGameplayEffectWithGrantedTags);
			OwnerCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(TagContainer);
		}
	}
}
