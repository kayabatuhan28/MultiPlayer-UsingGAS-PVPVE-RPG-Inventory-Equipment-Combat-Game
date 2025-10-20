﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ANS_AttackTrace.generated.h"


UCLASS()
class TPGAS_API UANS_AttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	FVector HalfSize = FVector(15.f, 15.f, 15.f);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> Query{};

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY()
	TArray<AActor*> HittedActors{};

	
};
