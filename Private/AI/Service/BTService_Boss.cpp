// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_Boss.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "TPGasGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPGasEnemyCharacter.h"
#include "Kismet/KismetStringLibrary.h"

UBTService_Boss::UBTService_Boss(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Boss Conditions";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_Boss::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (AiRef->FocusedActor == nullptr)
	{
		return;
	}

	if (CheckFocusedPlayerDie(AiRef) == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::FocusedTargetDie);
	}
	else if (AiRef->bIsBossPassiveWaiting == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::DefaultState);
	}
	else if (CheckCanGoEmptyState(AiRef))
	{
		AiRef->SetBehaviourState(EBehaviourStates::EmptyState);
	}
	else if (CheckCanSkillAttack(AiRef) == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::SkillAttack);
	}
	else if (CheckCanDodge(AiRef) == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::Dodge);
	}
	else if (CheckCanChase(AiRef) == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::ChasePlayer);
	}
	else if (CheckCanStrafe(AiRef) == true)
	{
		AiRef->SetBehaviourState(EBehaviourStates::Strafe);
	}
}

bool UBTService_Boss::CheckFocusedPlayerDie(ATPGasEnemyCharacter* AiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag DieTag = Tags.Abilities_Die;

	if (AiRef->FocusedActor == nullptr)
	{
		return false;
	}

	if (UAbilitySystemComponent* FocusedActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AiRef->FocusedActor))
	{
		if (FocusedActorASC->HasMatchingGameplayTag(DieTag))
		{
			return true;
		}
	}

	return false;
}

bool UBTService_Boss::CheckCanGoEmptyState(ATPGasEnemyCharacter* AiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag CloseDistanceSkillTag = Tags.Abilities_Boss_CloseDistanceSkill;
	FGameplayTag MidDistanceSkillTag = Tags.Abilities_Boss_MidDistanceSkill;
	FGameplayTag LongDistanceSkillTag = Tags.Abilities_Boss_LongDistanceSkill;
	FGameplayTag BossDodgeTag = Tags.Abilities_Boss_Dodge;

	bool cond1 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(CloseDistanceSkillTag);
	bool cond2 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(MidDistanceSkillTag);
	bool cond3 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(LongDistanceSkillTag);
	bool cond4 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(BossDodgeTag);

	if (cond1 || cond2 || cond3 || cond4)
	{
		return true;
	}

	return false;
}

bool UBTService_Boss::CheckCanChase(ATPGasEnemyCharacter* AiRef)
{
	if (AiRef->FocusedActor == nullptr)
	{
		return false;
	}

	bool cond1 = AiRef->GetDistanceTo(AiRef->FocusedActor) >= AiRef->StrafeDistance;
	if (cond1)
	{
		return true;
	}

	return false;
}

bool UBTService_Boss::CheckCanDodge(ATPGasEnemyCharacter* AiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag DodgeCooldownTag = Tags.Effects_Cooldown_Boss_Dodge;
	FGameplayTag AttackingTag = Tags.Effects_Attacking;
	
	if (AiRef->FocusedActor == nullptr)
	{
		return false;
	}

	if (UAbilitySystemComponent* FocusedActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AiRef->FocusedActor))
	{
		bool cond1 = FocusedActorASC->HasMatchingGameplayTag(AttackingTag);
		bool cond2 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(DodgeCooldownTag);
		bool cond3 = AiRef->GetDistanceTo(AiRef->FocusedActor) <= MaxDodgeActivateDistance;
		if (cond1 && !cond2 && cond3) 
		{
			return true;
		}
	}

	return false;
}

bool UBTService_Boss::CheckCanSkillAttack(ATPGasEnemyCharacter* AiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag SkillCooldownTag = Tags.Effects_Cooldown_Boss_SkillAttack;

	bool cond1 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(SkillCooldownTag);
	bool cond2 = AiRef->GetDistanceTo(AiRef->FocusedActor) <= MaxSkillDistance;
	
	if (cond1 == false && cond2)
	{
		return true;
	}

	return false;
}

bool UBTService_Boss::CheckCanStrafe(ATPGasEnemyCharacter* AiRef)
{
	const FTpGasGameplayTags& Tags = FTpGasGameplayTags::Get();
	FGameplayTag AttackCooldownTag = Tags.Effects_Cooldown_Boss_SkillAttack;
	
	bool cond1 = AiRef->GetDistanceTo(AiRef->FocusedActor) <= AiRef->StrafeDistance;
	bool cond2 = AiRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(AttackCooldownTag);

	if (cond1 && cond2)
	{
		return true;
	}
	return false;
}
