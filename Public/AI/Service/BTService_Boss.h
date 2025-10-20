// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_Boss.generated.h"



class ATPGasEnemyCharacter;

UCLASS()
class TPGAS_API UBTService_Boss : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UBTService_Boss(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float MaxSkillDistance = 1500.f;

	UPROPERTY(EditAnywhere)
	float MaxDodgeActivateDistance = 1000.f;

	bool CheckCanSkillAttack(ATPGasEnemyCharacter* AiRef);

	bool CheckCanStrafe(ATPGasEnemyCharacter* AiRef);

	bool CheckCanChase(ATPGasEnemyCharacter* AiRef);
	
	bool CheckCanDodge(ATPGasEnemyCharacter* AiRef);

	bool CheckCanGoEmptyState(ATPGasEnemyCharacter* AiRef);

	bool CheckFocusedPlayerDie(ATPGasEnemyCharacter* AiRef);
	
};





