// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPGasBaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TPGasEnemyCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AEnemyAIController;
class APatrolPath;

UENUM(BlueprintType)
enum EBehaviourStates
{
	DefaultState				UMETA(DisplayName = "DefaultState"),
	Patrol						UMETA(DisplayName = "Patrol"),
	Strafe						UMETA(DisplayName = "Strafe"),
	ChasePlayer					UMETA(DisplayName = "ChasePlayer"),
	Attack						UMETA(DisplayName = "Attack"),
	SkillAttack					UMETA(DisplayName = "SkillAttack"),
	FocusedTargetDie			UMETA(DisplayName = "FocusedTargetDie"),
	Escape						UMETA(DisplayName = "Escape"),
	ToCloseReact				UMETA(DisplayName = "ToCloseReact"),
	Dodge						UMETA(DisplayName = "Dodge"),
	EmptyState					UMETA(DisplayName = "EmptyState"),
};

UENUM(BlueprintType)
enum EAiMovementState
{
	Default					UMETA(DisplayName = "Default"),
	Walk					UMETA(DisplayName = "Walk"),
	Run						UMETA(DisplayName = "Run"),
	Sprint					UMETA(DisplayName = "Sprint"),
};

USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()
	
};

UCLASS()
class TPGAS_API ATPGasEnemyCharacter : public ATPGasBaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ATPGasEnemyCharacter();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEnemyAIController> EnemyAIController;

	virtual void AddCharacterAbilities() override;

	virtual int32 GetPlayerLevel_Implementation() override;

	// Some enemy uses weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Socket_WeaponAttach{};

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	virtual void MulticastHandleDeath(const FVector& DeathImpulse) override;

	// Enemy Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	bool bCanPlayDieMontage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	bool bCanStrafe = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	bool bCanEscape = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	FString EnemyName{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_LightAttack{};

	UPROPERTY(BlueprintReadOnly)
	int32 LightAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<float> Distance_LightAttack{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_HeavyAttack{};

	UPROPERTY(BlueprintReadOnly)
	int32 HeavyAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<float> Distance_HeavyAttack{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AData")
	float AttackDistance = 200.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_BasicHitReact{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_HeavyHitReact{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_Stun{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_Dodge{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	TArray<UAnimMontage*> M_ToCloseReact{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	UAnimMontage* M_Die{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	UAnimMontage* M_FirstSeenTaunt{};

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AData")
	AActor* FocusedActor{};
	
	UPROPERTY(BlueprintReadWrite, Category = "AData")
	TArray<AActor*> PotentialFocusedActor{};

	UPROPERTY(BlueprintReadWrite, Category = "AData")
	TEnumAsByte<EBehaviourStates> BehaviourState = EBehaviourStates::DefaultState;

	UFUNCTION(BlueprintCallable)
	void SetBehaviourState(TEnumAsByte<EBehaviourStates> NewState);

	UFUNCTION(BlueprintCallable)
	void SetNewFocusedActor(AActor* NewFocusedActor);

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	TArray<FGameplayTag> AttackList{}; // Light, Heavy or skill?

	UPROPERTY(BlueprintReadWrite, Category = "AData")
	int32 AttackListIndex{};

	UFUNCTION(BlueprintCallable)
	void SetNewAttackListData();

	UFUNCTION()
	void SetNewAttackMontageData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	UAnimMontage* M_SelectedAttackMontage{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
    FGameplayTag SelectedAttackTag{};

	UFUNCTION(BlueprintCallable)
	void SetAttackCd(float MinWaitTime, float MaxWaitTime );

	UPROPERTY(EditDefaultsOnly, Category = "AData")
	TSubclassOf<UGameplayEffect> GE_AttackCooldownClass{};

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMotionWarping(FVector WarpingLocation, FRotator WarpingRotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayDieMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AData")
	APatrolPath* PatrolPath{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float WalkSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float RunSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float SprintSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float StrafeDistance = 425.f;

	// For Boss
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float Skill_CloseDistance = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AData")
	float Skill_MidDistance = 900.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSecondPhaseActivated = false;

	UFUNCTION(BlueprintImplementableEvent)
	void SecondPhaseActivate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSkillAttackCooldown();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDodgeCooldown();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TArray<UAnimMontage*> M_Boss_CloseDistanceSkill{};

	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	int32 CloseDistanceSkillIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TArray<UAnimMontage*> M_Boss_MidDistanceSkill{};

	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	int32 MidDistanceSkillIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TArray<UAnimMontage*> M_Boss_LongDistanceSkill{};
	
	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	int32 LongDistanceSkillIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	bool bIsBossPassiveWaiting = false;
	// For Boss

	
	// For Summoned ai (Player or enemy can spawn summon)
	UPROPERTY(BlueprintReadWrite, Category = "AData")
	AActor* AiMasterActor{};

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyBody();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void Die(const FVector& DeathImpulse) override;
	
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AData")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	virtual void BurningTagChanged(const FGameplayTag CallBackTag, int32 NewCount) override;

	// Loot Spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	float LootSpawnChance = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AData")
	TArray<TSubclassOf<AActor>> PickableLoot{};

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
	

	
	
};
