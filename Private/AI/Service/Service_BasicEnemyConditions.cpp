// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Service_BasicEnemyConditions.h"

#include "AIController.h"
#include "TPGasGameplayTags.h"
#include "Character/TPGasEnemyCharacter.h"

UService_BasicEnemyConditions::UService_BasicEnemyConditions(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Basic Enemy Conditions";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UService_BasicEnemyConditions::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
}

void UService_BasicEnemyConditions::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn())
	{
		return;
	}

	ATPGasEnemyCharacter* AiRef = Cast<ATPGasEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AiRef == nullptr)
	{
		return;
	}

	if (AiRef->FocusedActor) // Active state, Enemy see player
	{
		if (CheckFocusedActorIsDie(AiRef) == true)
		{
			AiRef->SetBehaviourState(EBehaviourStates::FocusedTargetDie);
		}
		else if (CheckCanEscape(AiRef) == true)
		{
			if (AiRef->GetDistanceTo(AiRef->FocusedActor) <= ToCloseReactDistance && CheckCanToCloseReact(AiRef) == true)
			{
				AiRef->SetBehaviourState(EBehaviourStates::ToCloseReact);
			}
			else
			{
				AiRef->SetBehaviourState(EBehaviourStates::Escape);
			}
		}
		else if (CheckCanAttack(AiRef) == true)
		{
			AiRef->SetBehaviourState(EBehaviourStates::Attack);
		}
		else if (CheckCanStrafe(AiRef) == true)
		{
			AiRef->SetBehaviourState(EBehaviourStates::Strafe);
		}
		else if (CheckCanChasePlayer(AiRef) == true)
		{
			AiRef->SetBehaviourState(EBehaviourStates::ChasePlayer);
		}
	}
	else // Passive States
	{
		AiRef->SetBehaviourState(EBehaviourStates::Patrol);
	}
	
	
}

bool UService_BasicEnemyConditions::CheckCanToCloseReact(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag ToCloseReactCooldownTag = Tags.Cooldown_Ai_ToCloseReact;

	if (OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(ToCloseReactCooldownTag))
	{
		return false;
	}
	return true;
}

bool UService_BasicEnemyConditions::CheckFocusedActorIsDie(ATPGasEnemyCharacter* OwnerAiRef)
{
	if (ATPGasBaseCharacter* FocusedPlayer = Cast<ATPGasBaseCharacter>(OwnerAiRef->FocusedActor))
	{
		if (FocusedPlayer->bDead == true)
		{
			return true;
		}
	}

	return false;
}



bool UService_BasicEnemyConditions::bIsHitReacting(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag StunTag = Tags.Abilities_Status_Stun;
	FGameplayTag BasicHitReactTag = Tags.Abilities_Status_BasicHitReact;
	FGameplayTag HeavyHitReactTag = Tags.Abilities_Status_HeavyHitReact;
	
	bool cond1 = OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(StunTag);
	bool cond2 = OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(BasicHitReactTag);
	bool cond3 = OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(HeavyHitReactTag);
	
	if (cond1 || cond2 || cond3)
	{
		return true;
	}
	return false;
}

bool UService_BasicEnemyConditions::CheckCanEscape(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag ToCloseReactTag = Tags.Abilities_Combat_ToCloseReact;

	if (OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(ToCloseReactTag))
	{
		return false;
	}
	
	if (OwnerAiRef->bCanEscape && OwnerAiRef->GetDistanceTo(OwnerAiRef->FocusedActor) <= CanEscapeDistance)
	{
		return true;
	}
	return false;
}

bool UService_BasicEnemyConditions::CheckCanStrafe(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag ToCloseReactTag = Tags.Abilities_Combat_ToCloseReact;

	if (OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(ToCloseReactTag))
	{
		return false;
	}

	FGameplayTag AttackCooldownTag = Tags.Effects_Ai_AttackCooldown;
	
	bool cond1 = OwnerAiRef->GetDistanceTo(OwnerAiRef->FocusedActor) <= StrafeDistance;
	bool cond2 = OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(AttackCooldownTag);
	bool cond3 = bIsHitReacting(OwnerAiRef);
	

	if (cond1 && cond2 && OwnerAiRef->bCanStrafe == true && !cond3)
	{
		return true;
	}
	
	return false;
}

bool UService_BasicEnemyConditions::CheckCanChasePlayer(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag ToCloseReactTag = Tags.Abilities_Combat_ToCloseReact;

	if (OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(ToCloseReactTag))
	{
		return false;
	}
	
	bool cond1 = OwnerAiRef->GetDistanceTo(OwnerAiRef->FocusedActor) > StrafeDistance;
	bool cond2 = bIsHitReacting(OwnerAiRef);
	

	if (cond1 && !cond2)
	{
		return true;
	}
	
	return false;
}

bool UService_BasicEnemyConditions::CheckCanAttack(ATPGasEnemyCharacter* OwnerAiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get(); 
	FGameplayTag ToCloseReactTag = Tags.Abilities_Combat_ToCloseReact;

	if (OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(ToCloseReactTag))
	{
		return false;
	}
	
	FGameplayTag AttackCooldownTag = Tags.Effects_Ai_AttackCooldown;

	bool cond1 = !OwnerAiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(AttackCooldownTag);
	bool cond2 = OwnerAiRef->GetDistanceTo(OwnerAiRef->FocusedActor) <= OwnerAiRef->AttackDistance;
	bool cond3 = bIsHitReacting(OwnerAiRef);

	if (cond1 && cond2 && !cond3)
	{
		return true;
	}
	
	return false;
}
