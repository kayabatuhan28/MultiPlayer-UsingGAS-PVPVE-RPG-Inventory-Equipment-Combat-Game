// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ApplyRemoveGameplayEffect.generated.h"


class UGameplayEffect;

UCLASS()
class TPGAS_API UANS_ApplyRemoveGameplayEffect : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AppliedGameplayEffectClass{};

	UPROPERTY(EditAnywhere)
	FGameplayTag RemovedGameplayEffectWithGrantedTags{};
};
