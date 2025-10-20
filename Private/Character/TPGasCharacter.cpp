// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/TPGasCharacter.h"

#include "AbilitySystemComponent.h"
#include "TPGasGameplayTags.h"
#include "AbilitySystem/TPGasAbilitySystemComponent.h"
#include "AbilitySystem/TPGasAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/SkillActor/TargetArea.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Combat/Component/TPGasCombatComp.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Combat/Component/TPGasCombatComp.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/TPGasPlayerController.h"
#include "GameFramework/TPGasPlayerState.h"
#include "InventorySystem/EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/TPGasHUD.h"
#include "UI/Widget/PlayerOnHeadWidget.h"
#include "UI/Widget/TPGasUserWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATPGasCharacter::ATPGasCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	CombatComp = CreateDefaultSubobject<UTPGasCombatComp>("CombatComp");

	Head = CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	Head->SetupAttachment(GetMesh());
	
	Body = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	Body->SetupAttachment(GetMesh());
	//Body->SetIsReplicated(true);

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>("Legs");
	Legs->SetupAttachment(GetMesh());
	//Legs->SetIsReplicated(true);

	Foot = CreateDefaultSubobject<USkeletalMeshComponent>("Foot");
	Foot->SetupAttachment(GetMesh());

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>("Arms");
	Arms->SetupAttachment(GetMesh());

	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>("Gloves");
	Gloves->SetupAttachment(GetMesh());

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
	
}

void ATPGasCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(CombatComp->GetOwner()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CombatComp Owner Not Valid In TPGasCharacter.cpp !!!"));
	}

	
}

void ATPGasCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateTargetAreaLocation();
}

void ATPGasCharacter::InitAbilityActorInfo()
{
	ATPGasPlayerState* TPGasPlayerState = GetPlayerState<ATPGasPlayerState>();
	if (!TPGasPlayerState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("InitAbilityActorInfo: PlayerState is null, skipping"));

		return;
	}

	UAbilitySystemComponent* ASC = TPGasPlayerState->GetAbilitySystemComponent();
	UAttributeSet* AS = TPGasPlayerState->GetAttributeSet();

	if (!ASC || !AS)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red,
			TEXT("InitAbilityActorInfo: ASC or AttributeSet is null, skipping"));

		return;
	}

	// ASC → Character bind
	ASC->InitAbilityActorInfo(TPGasPlayerState, this);

	
	if (UTPGasAbilitySystemComponent* TPGasASC = Cast<UTPGasAbilitySystemComponent>(ASC))
	{
		TPGasASC->AbilityActorInfoSet();
	}

	AbilitySystemComponent = ASC;
	AttributeSet = AS;

	// UI Setup
	if (ATPGasPlayerController* TPGasPC = Cast<ATPGasPlayerController>(GetController()))
	{
		if (TPGasPC->IsLocalController()) 
		{
			if (ATPGasHUD* TPGasHUD = Cast<ATPGasHUD>(TPGasPC->GetHUD()))
			{
				// Retry call (will try again after 1 tick if the HUD is not ready)
				TPGasHUD->TryInitOverlay();
			}
		}
	}

	// Apply initial attributes (set on the server side and replicated to clients via OnRep)
	InitializeDefaultAttributes();

	
}



int32 ATPGasCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);

	return TPGasPS->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ATPGasCharacter::GetXP_Implementation() const
{
	const ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);
	
	return TPGasPS->GetXP();
}

int32 ATPGasCharacter::GetSpellPoints_Implementation() const
{
	ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);

	return TPGasPS->GetSpellPoints();
}

void ATPGasCharacter::AddToXP_Implementation(int32 InXP)
{
	ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);

	TPGasPS->AddToXP(InXP);
}

void ATPGasCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);
	TPGasPS->AddToLevel(InPlayerLevel);
	OnLevelChanged.Broadcast(TPGasPS->GetPlayerLevel());

	if (UTPGasAbilitySystemComponent* AuraASC = Cast<UTPGasAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatues(TPGasPS->GetPlayerLevel());
	}
}

void ATPGasCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);

	TPGasPS->AddToSpellPoints(InSpellPoints);
}

int32 ATPGasCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	const ATPGasPlayerState* TPGasPS = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPS);

	return TPGasPS->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void ATPGasCharacter::LevelUp_Implementation()
{
	IPlayerInterface::LevelUp_Implementation();
}

void ATPGasCharacter::BindOnHeadWidgetDelegate()
{
	if (UTPGasUserWidget* AuraUserWidget = Cast<UTPGasUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UTPGasAttributeSet* TPGasAS = Cast<UTPGasAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TPGasAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TPGasAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		// Initialize
		OnHealthChanged.Broadcast(TPGasAS->GetHealth());
		OnMaxHealthChanged.Broadcast(TPGasAS->GetMaxHealth());

		AbilitySystemComponent->RegisterGameplayTagEvent(FTpGasGameplayTags::Get().Effects_Buff_NatureStaffSkill4, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ATPGasCharacter::NatureStaffSkill4BuffTagChanged
		);
	}
}

void ATPGasCharacter::NatureStaffSkill4BuffTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	if (UPlayerOnHeadWidget* PlayerOnHeadWidget = Cast<UPlayerOnHeadWidget>(HealthBar->GetUserWidgetObject()))
	{
		PlayerOnHeadWidget->SetNatureStaffSkill4Icon(NewCount);
		NatureStaffSkill4BuffTagChangeEvent(NewCount);
	}
}

void ATPGasCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPGasCharacter, TraceAimImpactPoint)
}

void ATPGasCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();

	BindOnHeadWidgetDelegate();
}

void ATPGasCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();

	BindOnHeadWidgetDelegate();
}

void ATPGasCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	PlayerAfterDie();
}

void ATPGasCharacter::MulticastHandleDeath(const FVector& DeathImpulse)
{
	bDead = true;
}

int32 ATPGasCharacter::GetPlayerLevel_Implementation()
{
	const ATPGasPlayerState* TPGasPlayerState = GetPlayerState<ATPGasPlayerState>();
	check(TPGasPlayerState);
	return TPGasPlayerState->GetPlayerLevel();
}

void ATPGasCharacter::Server_SetBodySkeletalMesh_Implementation(USkeletalMesh* NewSkeletalMesh, USkinnedAsset* NewSkinnedAsset, EEquipArmorType InEquipActorType)
{
	Multicast_SetBodySkeletalMesh(NewSkeletalMesh, NewSkinnedAsset, InEquipActorType);
}

void ATPGasCharacter::Multicast_SetBodySkeletalMesh_Implementation(USkeletalMesh* NewSkeletalMesh, USkinnedAsset* NewSkinnedAsset, EEquipArmorType InEquipActorType)
{
	USkeletalMesh* SelectedNewMesh{};
	USkinnedAsset* SelectedNewSkinnedAsset{};
	if (NewSkeletalMesh == nullptr || NewSkinnedAsset == nullptr)
	{
		if (InEquipActorType == EEquipArmorType::Head)
		{
			SelectedNewMesh = DefaultBodyParts[0];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[0];
		}
		else if (InEquipActorType == EEquipArmorType::Body)
		{
			SelectedNewMesh = DefaultBodyParts[1];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[1];
		}
		else if (InEquipActorType == EEquipArmorType::Legs)
		{
			SelectedNewMesh = DefaultBodyParts[2];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[2];
		}
		else if (InEquipActorType == EEquipArmorType::Arm)
		{
			SelectedNewMesh = DefaultBodyParts[3];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[3];
		}
		else if (InEquipActorType == EEquipArmorType::Foot)
		{
			SelectedNewMesh = DefaultBodyParts[4];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[4];
		}
		else if (InEquipActorType == EEquipArmorType::Glove)
		{
			SelectedNewMesh = DefaultBodyParts[5];
			SelectedNewSkinnedAsset = DefaultSkinnedAssets[5];
		}
	}
	else
	{
		SelectedNewMesh = NewSkeletalMesh;
		SelectedNewSkinnedAsset = NewSkinnedAsset;
	}
	
	switch (InEquipActorType)
	{
	case EEquipArmorType::Head:
		Head->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Head->SetSkeletalMeshAsset(SelectedNewMesh);
		break;
	case EEquipArmorType::Body:
		Body->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Body->SetSkeletalMeshAsset(SelectedNewMesh);
		break;
	case EEquipArmorType::Legs:
		Legs->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Legs->SetSkeletalMeshAsset(SelectedNewMesh);
		break;
	case EEquipArmorType::Arm:
		Arms->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Arms->SetSkeletalMeshAsset(SelectedNewMesh);
		break;
	case EEquipArmorType::Foot:
		Foot->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Foot->SetSkeletalMeshAsset(SelectedNewMesh);
		break;
	case EEquipArmorType::Glove:
		Gloves->SetSkinnedAssetAndUpdate(SelectedNewSkinnedAsset, true);
		Gloves->SetSkeletalMeshAsset(SelectedNewMesh);
		break;	
	default:
		break;
	}
}

float ATPGasCharacter::CheckDirectionRotate()
{
	ATPGasPlayerController* PlayerController = Cast<ATPGasPlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		return 180.f;
	}
	
	float LeftRight = PlayerController->ActionValueX;
	float ForwardBackward = PlayerController->ActionValueY;
	
	if (ForwardBackward == 1)
	{
		if (LeftRight == 1) // W+D
		{
			return 45.f;
		}
		if (LeftRight == -1) // W+A
		{
			return -45.f;
		}
		return 0.f; // W
	}

	if (ForwardBackward == -1)
	{
		if (LeftRight == 1) // S+D
		{
			return 135.f;
		}
		if (LeftRight == -1) // S+A
		{
			return -135.f;
		}
		return 180.f;
	}

	if (LeftRight == 1) 
	{
		if (ForwardBackward == 1) // D+W
		{
			return -45.f;
		}
		if (ForwardBackward == -1) // D+S
		{
			return -135.f;
		}
		return 90.f;
	}
	
	if (LeftRight == -1) 
	{
		if (ForwardBackward == 1) // A+W
		{
			return 45.f;
		}
		if (ForwardBackward == -1) // A+S
		{
			return  135.f;
		}
		if (ForwardBackward == 0) // A
		{
			return -90.f;
		}
	}

	return 0;
}

void ATPGasCharacter::CalculateAnimDirection()
{
	float HoldRotate = CheckDirectionRotate();
	FRotator Rotator;
	Rotator.Pitch = GetActorRotation().Pitch;
	Rotator.Roll = GetActorRotation().Roll;
	Rotator.Yaw = GetFollowCamera()->GetComponentRotation().Yaw + HoldRotate;
	SetActorRotationReplicated(Rotator);
}

void ATPGasCharacter::ShowTargetArea(FVector InTargetAreaScale, float TargetAreaMaxDistance)
{
	if (!IsValid(TargetArea) && HasAuthority() == true)
	{
		//TargetingCircleMaxDistance = TargetAreaMaxDistance;
		Client_SpawnTargetArea(InTargetAreaScale);
	}
}

void ATPGasCharacter::Client_SpawnTargetArea_Implementation(FVector InTargetAreaScale)
{
	TargetArea = GetWorld()->SpawnActor<ATargetArea>(TargetAreaClass);
	TargetArea->SetTargetAreaScale(InTargetAreaScale);
}

void ATPGasCharacter::HideTargetArea()
{
	if (IsValid(TargetArea))
	{
		TargetArea->Destroy();
	}
}

void ATPGasCharacter::UpdateTargetAreaLocation()
{
	if (IsValid(TargetArea))
	{
		SetAimImpactPoint();
		Server_UpdateTargetAreaLocation(TraceAimImpactPoint);

		
		/* For now closed
		 * 
		float DistanceTwoVectors = (GetActorLocation() - TraceAimImpactPoint).Length();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("DistanceTwoVectors = %f, TraceAimImpactPoint = %s "), DistanceTwoVectors, *TraceAimImpactPoint.ToString()));
		if (DistanceTwoVectors >= TargetingCircleMaxDistance)
		{
			FVector NewLocation = GetActorLocation() + GetActorForwardVector() * TargetingCircleMaxDistance;
			NewLocation.Z = 0.f;
			Server_SetReplicatedTraceAimImpactPoint(NewLocation);
			Server_UpdateTargetAreaLocation(TraceAimImpactPoint);	
		}
		else
		{
			Server_UpdateTargetAreaLocation(TraceAimImpactPoint);	
		}
		*/
			
	}
}

void ATPGasCharacter::Server_UpdateTargetAreaLocation_Implementation(FVector NewLoc)
{
	Multicast_UpdateTargetAreaLocation(NewLoc);
}

void ATPGasCharacter::Multicast_UpdateTargetAreaLocation_Implementation(FVector NewLoc)
{
	if (IsValid(TargetArea))
	{
		TargetArea->SetActorLocation(NewLoc);
	}
}

void ATPGasCharacter::Server_SetReplicatedTraceAimImpactPoint_Implementation(FVector NewTraceAimImpactPoint)
{
	Multicast_SetReplicatedTraceAimImpactPoint(NewTraceAimImpactPoint);
}

void ATPGasCharacter::Multicast_SetReplicatedTraceAimImpactPoint_Implementation(FVector NewTraceAimImpactPoint)
{
	TraceAimImpactPoint = NewTraceAimImpactPoint;
}

void ATPGasCharacter::SetAimImpactPoint()
{
	Server_SetAimImpactPoint();
}

void ATPGasCharacter::Server_SetAimImpactPoint_Implementation()
{
	Multicast_SetAimImpactPoint();
}

void ATPGasCharacter::Multicast_SetAimImpactPoint_Implementation()
{
	FVector OutImpactPoint;

	float ForwardValue = 8000.f;
	FVector TraceStart = GetFollowCamera()->GetComponentLocation();
	FVector TraceEnd = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * ForwardValue);
	
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (HitResult.bBlockingHit)
	{
		OutImpactPoint = HitResult.ImpactPoint;
	}
	else
	{
		OutImpactPoint = HitResult.TraceEnd;	
	}

	/*
	DrawDebugLine(GetWorld(),
		TraceStart,
		TraceEnd,
		HitResult.bBlockingHit ? FColor::Green : FColor::Red,
		false,
		5.0f,
		0,
		0.f);
	*/	

	OutImpactPoint.Z = 0.f;
	TraceAimImpactPoint = OutImpactPoint;
	GEngine->AddOnScreenDebugMessage(1,5.f,FColor::Green,
		FString::Printf(TEXT("%s"), *TraceAimImpactPoint.ToString()));
}





