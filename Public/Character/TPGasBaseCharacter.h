// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interface/ICombatInterface.h"
#include "TPGasBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UDamageTextComponent;

UCLASS()
class TPGAS_API ATPGasBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IICombatInterface
{
	GENERATED_BODY()

public:
	
	ATPGasBaseCharacter();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer ClearTagWhenDie{};

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Combat Interface Functions Begin
	// -------------------------------------------------------------------------------------------------------------------------------
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FOnASCRegistered& GetOnAscRegisteredDelegate() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	// Combat Interface Functions End
	// -------------------------------------------------------------------------------------------------------------------------------

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UFUNCTION(NetMulticast, Reliable)
	void ShowDamageNumber_Multicast(float DamageAmount, ACharacter* TargetCharacter, bool bIsHealing,  bool bInIsCriticalHit);

	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsHealing, bool bInIsCriticalHit);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UFUNCTION(BlueprintCallable)
	void SetActorRotationReplicated(FRotator NewRot);

	UFUNCTION(Server, Reliable)
	void Server_SetActorRotationReplicated(FRotator NewRot);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetActorRotationReplicated(FRotator NewRot);

	// Replicated Movement Speed Set
	UFUNCTION(BlueprintCallable)
	void SetMovementSpeedReplicated(float NewSpeed);
	
	UFUNCTION(Server, Reliable)
	void Server_SetMovementSpeed(float NewSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMovementSpeed(float NewSpeed);

	virtual void BurningTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void BurningTagChangeEvent(int32 NewCount);

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsFlameThrowing = false;

	UPROPERTY(EditAnywhere, Category = "AData")
	float GivenXP = 10.f;

	bool bDead = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_Heal;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayReplicatedMontage(UAnimMontage* InMontage);

	

	


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	// Baslangicta primary attribute datalarini initialize etmek icin (strength, intelligence vs.)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Secondary Attributeyi primary attributeyi initialize ettikten sonra initialize ederiz cünkü secondary attributeler (armor,critical hit chance vs.)
	// Primary Attributelere gore degisen attributelerdir (Vigor artinca hp nin artmasi, healthregenin artmasi vs gibi)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// Vital Attributes : Health ve Mana, Secondary Attributes setlendikten sonra bu effecti apply ederiz Secondary ile maxhealth max mana setlendigi icin
	// Ardından CurrentHealth ve CurrentManayida setleyebiliriz gameplayeffectte direk maxhealthi override etmek gibi health icin
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const;

	virtual void InitializeDefaultAttributes() const;
	
	virtual void AddCharacterAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::NoType;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;


private:

	

};
