// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPGasEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/TPGasAbilitySystemLibrary.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "AI/EnemyAIController.h"
#include "AI/PatrolPath.h"
#include "Algo/RandomShuffle.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "UI/Widget/TPGasUserWidget.h"

ATPGasEnemyCharacter::ATPGasEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPGasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTPGasAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetupAttachment(GetMesh(), Socket_WeaponAttach);
}

void ATPGasEnemyCharacter::AddCharacterAbilities()
{
	UTPGasAbilitySystemComponent* TPGasASC = CastChecked<UTPGasAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	TPGasASC->AddCharacterAbilities(StartupAbilities, Level);
	TPGasASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

int32 ATPGasEnemyCharacter::GetPlayerLevel_Implementation()
{
	return Level;
}

void ATPGasEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	EnemyAIController = Cast<AEnemyAIController>(NewController);
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);
	
	
}

void ATPGasEnemyCharacter::MulticastHandleDeath(const FVector& DeathImpulse)
{
	Super::MulticastHandleDeath(DeathImpulse);

	HealthBar->SetVisibility(false);
}

void ATPGasEnemyCharacter::SetBehaviourState(TEnumAsByte<EBehaviourStates> NewState)
{
	if (EnemyAIController == nullptr) return;

	EnemyAIController->GetBlackboardComponent()->SetValueAsEnum("State", NewState.GetIntValue());
	BehaviourState = NewState;
}

void ATPGasEnemyCharacter::SetNewFocusedActor(AActor* NewFocusedActor)
{
	if (EnemyAIController == nullptr) return;

	EnemyAIController->GetBlackboardComponent()->SetValueAsObject("FocusedActor", NewFocusedActor);
	FocusedActor = NewFocusedActor;
}

void ATPGasEnemyCharacter::SetNewAttackListData()
{
	if (AttackListIndex + 1 > AttackList.Num() - 1) // attacklist finish, shuffle list
	{
		Algo::RandomShuffle(AttackList);
		AttackListIndex = 0;
		SetNewAttackMontageData();
	}
	else
	{
		AttackListIndex++;
		SetNewAttackMontageData();
	}
}

void ATPGasEnemyCharacter::SetNewAttackMontageData()
{
	const FTpGasGameplayTags& GameplayTags = FTpGasGameplayTags::Get();
	
	if (AttackList[AttackListIndex] == GameplayTags.Abilities_Combat_LightAttack)
	{
		const int32 TemLightAttackIndex = FMath::RandRange(0, M_LightAttack.Num() - 1);
		M_SelectedAttackMontage = M_LightAttack[TemLightAttackIndex];
		AttackDistance = Distance_LightAttack[TemLightAttackIndex];
		SelectedAttackTag = GameplayTags.Abilities_Combat_LightAttack;
	}
	else if (AttackList[AttackListIndex] == GameplayTags.Abilities_Combat_HeavyAttack)
	{
		const int32 TemHeavyAttackIndex = FMath::RandRange(0, M_HeavyAttack.Num() - 1);
		M_SelectedAttackMontage = M_HeavyAttack[TemHeavyAttackIndex];
		AttackDistance = Distance_HeavyAttack[TemHeavyAttackIndex];
		SelectedAttackTag = GameplayTags.Abilities_Combat_HeavyAttack;
	}
}

void ATPGasEnemyCharacter::SetAttackCd(float MinWaitTime, float MaxWaitTime)
{
	if (AbilitySystemComponent == nullptr) return;
	
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Event.Ai.AttackCooldown");
	float WaitTime = FMath::RandRange(MinWaitTime, MaxWaitTime);

	FGameplayEffectContextHandle ContextHandle;
	FGameplayEffectSpecHandle DamageSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE_AttackCooldownClass, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, EventTag, WaitTime);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

void ATPGasEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (HasAuthority()) // Clientlerde crashi önlemek icin ekledik 
	{
		// Enemylere ability verir.
		UTPGasAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (UTPGasUserWidget* AuraUserWidget = Cast<UTPGasUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	
	if (const UTPGasAttributeSet* TPGasAS = Cast<UTPGasAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TPGasAS->GetHealthAttribute()).AddLambda(
			[this, TPGasAS](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
				
				if (ActorHasTag("Boss"))
				{
					if (TPGasAS->GetHealth() <= TPGasAS->GetMaxHealth() / 2.f)
					{
						SecondPhaseActivate();
					}
				}
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TPGasAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		
		AbilitySystemComponent->RegisterGameplayTagEvent(FTpGasGameplayTags::Get().Effects_Debuff_Burning, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ATPGasEnemyCharacter::BurningTagChanged
		);
		

		// Initialize
		OnHealthChanged.Broadcast(TPGasAS->GetHealth());
		OnMaxHealthChanged.Broadcast(TPGasAS->GetMaxHealth());
	}

	if (PatrolPath && EnemyAIController)
	{
		FVector PatrolPointStart = PatrolPath->SplineComponent->GetSplinePointAt(0, ESplineCoordinateSpace::World).Position;
		EnemyAIController->GetBlackboardComponent()->SetValueAsVector("PatrolPoint", PatrolPointStart);
	}
	
}

void ATPGasEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTPGasAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

	//OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void ATPGasEnemyCharacter::Die(const FVector& DeathImpulse)
{
	SpawnLoot();

	Super::Die(DeathImpulse);

	if (EnemyAIController && AbilitySystemComponent)
	{
		if (UBrainComponent* BrainComponent = EnemyAIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(FString("Dead"));
			AbilitySystemComponent->CancelAllAbilities();
		}
	}

	if (bCanPlayDieMontage == true)
	{
		PlayDieMontage();
	}

	DestroyBody();
}

void ATPGasEnemyCharacter::InitializeDefaultAttributes() const
{
	UTPGasAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ATPGasEnemyCharacter::BurningTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	Super::BurningTagChanged(CallBackTag, NewCount);

	BurningTagChangeEvent(NewCount);
}


