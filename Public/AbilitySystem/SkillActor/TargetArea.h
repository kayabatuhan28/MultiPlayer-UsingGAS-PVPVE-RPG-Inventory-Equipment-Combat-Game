// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetArea.generated.h"

UCLASS()
class TPGAS_API ATargetArea : public AActor
{
	GENERATED_BODY()

public:
	ATargetArea();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetTargetAreaScale(FVector TargetAreaScale);

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
