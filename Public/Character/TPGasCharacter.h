// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TPGasBaseCharacter.h"
#include "Interface/PlayerInterface.h"
#include "Logging/LogMacros.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TPGasCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UTPGasCombatComp;
class AInv_EquipActor;
class ATargetArea;
class UWidgetComponent;
class UPlayerOnHeadWidget;



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATPGasCharacter : public ATPGasBaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	ATPGasCharacter();

	// Player Interface Functions
	// -------------------------------------------------------------------
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 InLevel) const override;
	virtual void LevelUp_Implementation() override;
	// Player Interface Functions
	// -------------------------------------------------------------------

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnLevelChanged;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetStaminaRegenStatus(bool bIsStart);

	UFUNCTION(BlueprintImplementableEvent)
	void SetManaRegenStatus(bool bIsStart);

	UFUNCTION(BlueprintCallable)
	void BindOnHeadWidgetDelegate();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Die Events
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void MulticastHandleDeath(const FVector& DeathImpulse) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerAfterDie();

	UPROPERTY(BlueprintReadOnly)
	UTPGasCombatComp* CombatComp;

	// Combat Interface Functions Begin
	// -------------------------------------------------------------------------------------------------------------------------------
	virtual int32 GetPlayerLevel_Implementation() override;
	// Combat Interface Functions End
	// -------------------------------------------------------------------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Head;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Legs;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Foot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Arms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Gloves;

	UPROPERTY(EditDefaultsOnly)
	TArray<USkeletalMesh*> DefaultBodyParts{};

	UPROPERTY(EditDefaultsOnly)
	TArray<USkinnedAsset*> DefaultSkinnedAssets{};

	UFUNCTION(Server, Reliable)
	void Server_SetBodySkeletalMesh(USkeletalMesh* NewSkeletalMesh, USkinnedAsset* NewSkinnedAsset, EEquipArmorType InEquipActorType);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetBodySkeletalMesh(USkeletalMesh* NewSkeletalMesh, USkinnedAsset* NewSkinnedAsset, EEquipArmorType InEquipActorType);

	float CheckDirectionRotate();

	UFUNCTION(BlueprintCallable)
	void CalculateAnimDirection();

	// Target Area
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector TraceAimImpactPoint = FVector::ZeroVector;

	UFUNCTION(BlueprintCallable)
	void ShowTargetArea(FVector InTargetAreaScale = FVector(0.8, 0.8, 0.8), float TargetAreaMaxDistance = 700.f);

	UFUNCTION(Client, Reliable)
	void Client_SpawnTargetArea(FVector InTargetAreaScale);

	UFUNCTION(BlueprintCallable)
	void HideTargetArea();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATargetArea> TargetAreaClass;

	UPROPERTY()
	TObjectPtr<ATargetArea> TargetArea;

	void UpdateTargetAreaLocation();

	void SetAimImpactPoint();

	UFUNCTION(Server, Reliable)
	void Server_SetAimImpactPoint();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAimImpactPoint();

	UFUNCTION(Server, Reliable)
	void Server_SetReplicatedTraceAimImpactPoint(FVector NewTraceAimImpactPoint);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetReplicatedTraceAimImpactPoint(FVector NewTraceAimImpactPoint);

	UFUNCTION(Server, Reliable)
	void Server_UpdateTargetAreaLocation(FVector NewLoc);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateTargetAreaLocation(FVector NewLoc);

	UPROPERTY(BlueprintReadWrite)
	float TargetingCircleMaxDistance = 700.f;

	void NatureStaffSkill4BuffTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void NatureStaffSkill4BuffTagChangeEvent(int32 NewCount);

	UPROPERTY(BlueprintReadWrite, Category = "AData")
	TArray<AActor*> SummonedActors{};

	UFUNCTION(BlueprintImplementableEvent)
	void Testkee(int32 NewLevel);

protected:
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	virtual void Tick(float DeltaSeconds) override;

private:
	// AuraBaseCharacter classindan override ettik, 
	void InitAbilityActorInfo() override;

	

public:
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};


