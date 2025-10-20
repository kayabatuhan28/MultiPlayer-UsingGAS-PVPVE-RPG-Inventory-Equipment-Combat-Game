// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Service_BasicEnemyConditions.generated.h"

class ATPGasEnemyCharacter;

UCLASS()
class TPGAS_API UService_BasicEnemyConditions : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	
	UService_BasicEnemyConditions(const FObjectInitializer& ObjectInitializer);	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category= "float")
	float StrafeDistance = 400.f;

	UPROPERTY(EditAnywhere, Category= "float")
	float CanEscapeDistance = 600.f;

	UPROPERTY(EditAnywhere, Category= "float")
	float ToCloseReactDistance = 300.f;

	bool CheckCanStrafe(ATPGasEnemyCharacter* OwnerAiRef);

	bool CheckCanChasePlayer(ATPGasEnemyCharacter* OwnerAiRef);
	
	bool CheckCanAttack(ATPGasEnemyCharacter* OwnerAiRef);

	bool CheckFocusedActorIsDie(ATPGasEnemyCharacter* OwnerAiRef);
	
	bool bIsHitReacting(ATPGasEnemyCharacter* OwnerAiRef);

	bool CheckCanEscape(ATPGasEnemyCharacter* OwnerAiRef);

	bool CheckCanToCloseReact(ATPGasEnemyCharacter* OwnerAiRef);
	
};






