// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Ball/Ball.h"
#include "GameFramework/Controller.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"

void ABaseCharacter::MontageStarted()
{
	is_montage_started_ = true;
	is_montage_ended_ = false;
}

void ABaseCharacter::MontageEnded()
{
	is_montage_ended_ = true;
}

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetSuperSettings();
	SetPlayerAttributes();
	SetCapsuleComponent();
	SetCharacterMovement();
	LoadDataTable();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = false;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsClicking)
	{
		// Gauge To Power
		Gauge = Gauge < 100.0f ? Gauge + DeltaTime * GaugeSpeed : 100.0f;

		// Timing To Accuracy
		TimingTimer += DeltaTime;
		TimingAccuracy = TimingTimer / TimingMax;
	}
	else
	{
		Gauge = 0;
		TimingAccuracy = -1.0f;
		TimingTimer = 0.0f;
		TimingMax = 0.0f;
	}

	// Smoothing between Walk & Sprint
	if (!bIsSprint && fabs(GetCharacterMovement()->MaxWalkSpeed - WalkSpeed) > 1.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed - 300.0f * DeltaTime;
	}
	else if (!bIsSprint && fabs(GetCharacterMovement()->MaxWalkSpeed - WalkSpeed) <= 1.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ABaseCharacter::SetSuperSettings()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ABaseCharacter::SetPlayerAttributes()
{
	PlayerTurn = EPlayerTurn::PT_SERVICE;

	GaugeSpeed = 100.0f;

	WalkSpeed = 500.0f;
	SprintSpeed = 700.0f;

	OffenceMode = EOffenceMode::OM_NONE;
	DefenceMode = EDefenceMode::DM_NONE;

	Direction = FName(TEXT("Left"));

	bIsClicking = false;
	bIsSprint = false; 

	Gauge = 0.0f;
	TimingAccuracy = 0.0f;
	TimingTimer = 0.0f;
	TimingMax = 0.0f;
}

void ABaseCharacter::SetCapsuleComponent()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.0f);
	GetCapsuleComponent()->SetRelativeLocation({ 0.f, 0.f, -90.f });
}

void ABaseCharacter::SetCharacterMovement()
{
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

bool ABaseCharacter::LoadDataTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimOffsetDataObject(TEXT("/Game/Data/AnimationOffset.AnimationOffset"));
	if (AnimOffsetDataObject.Succeeded())
	{
		AnimOffsetData = AnimOffsetDataObject.Object;

		auto ServiceRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Service")), TEXT("Fail"));
		ServiceOffsetMap.Add(FName("Spoon"), ServiceRow->Spoon);
		ServiceOffsetMap.Add(FName("Floating"), ServiceRow->Floating);
		ServiceOffsetMap.Add(FName("Jump"), ServiceRow->Jump);

		auto ReceiveRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Receive")), TEXT("Fail"));
		ReceiveOffsetMap.Add(FName("Back"), ReceiveRow->Back);
		ReceiveOffsetMap.Add(FName("Front"), ReceiveRow->Front);
		ReceiveOffsetMap.Add(FName("Right"), ReceiveRow->Right);
		ReceiveOffsetMap.Add(FName("Left"), ReceiveRow->Left);

		auto DigRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Dig")), TEXT("Fail"));
		DigOffsetMap.Add(FName("Front"), DigRow->Front);
		DigOffsetMap.Add(FName("Right"), DigRow->Right);
		DigOffsetMap.Add(FName("Left"), DigRow->Left);

		auto BlockRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Block")), TEXT("Fail"));
		BlockOffsetMap.Add(FName("Front"), BlockRow->Front);
		BlockOffsetMap.Add(FName("Right"), BlockRow->Right);
		BlockOffsetMap.Add(FName("Left"), BlockRow->Left);

		auto SpikeRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Spike")), TEXT("Fail"));
		SpikeOffsetMap.Add(FName("FullSpike"), SpikeRow->FullSpike);
		SpikeOffsetMap.Add(FName("SemiSpike"), SpikeRow->SemiSpike);

		auto FloatingRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Floating")), TEXT("Fail"));
		FloatingOffsetMap.Add(FName("Back"), FloatingRow->Back);
		FloatingOffsetMap.Add(FName("Front"), FloatingRow->Front);
		FloatingOffsetMap.Add(FName("Right"), FloatingRow->Right);
		FloatingOffsetMap.Add(FName("Left"), FloatingRow->Left);

		auto TossRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Toss")), TEXT("Fail"));
		TossOffsetMap.Add(FName("Front"), TossRow->Front);
		TossOffsetMap.Add(FName("Right"), TossRow->Right);
		TossOffsetMap.Add(FName("Left"), TossRow->Left);

		auto PassRow = AnimOffsetData->FindRow<FAnimationOffsetData>(FName(TEXT("Pass")), TEXT("Fail"));
		PassOffsetMap.Add(FName("Back"), PassRow->Back);
		PassOffsetMap.Add(FName("Front"), PassRow->Front);
		PassOffsetMap.Add(FName("Right"), PassRow->Right);
		PassOffsetMap.Add(FName("Left"), PassRow->Left);
		
		return true;
	}
	else
	{
		return false;
	}
		
}

void ABaseCharacter::ServiceFloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Float Ball"));
	if (Ball)
	{
		// Detach From Hand
		Ball->PushAndUpdateBallState(EBallState::eFloatToService);
		FDetachmentTransformRules rule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		Ball->GetProjectileComp()->SetActive(true);
		Ball->DetachFromActor(rule);

		FVector Offset = *ServiceOffsetMap.Find(ServiceMode);
		Offset = RotateOffsetToCurrentDirection(Offset);
		FVector StartPos = Ball->GetActorLocation();
		FVector EndPos = GetActorLocation() + Offset;

		// Floating Ball to Service Mode
		float power = 1.0f;
		if (ServiceMode == FName("Spoon"))
		{
			power = 1.2f;
		}
		if (ServiceMode == FName("Floating"))
		{
			power = 0.8f;
		}
		if (ServiceMode == FName("Jump"))
		{
			power = 0.8f;
		}

		Ball->ReceiveMovement(power, StartPos, EndPos, EBallState::eFloatToService);
	}
}

void ABaseCharacter::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));

	if (Ball)
	{
		FVector StartPos = Ball->GetActorLocation();
		FVector EndPos = { 2380, 1470, 20 };

		if (ServiceMode == FName("Spoon"))
		{
			Ball->SpoonServiceMovement(1.0, StartPos, EndPos, EBallState::eTurnOver);
		}
		if (ServiceMode == FName("Floating"))
		{
			Ball->FloatingServiceMovement(1.0, StartPos, EndPos, EBallState::eTurnOver);
		}
		if (ServiceMode == FName("Jump"))
		{
			Ball->JumpServiceMovement(1.0, StartPos, EndPos, EBallState::eTurnOver);
		}
	}
	
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABaseCharacter::DigBall()
{
	UE_LOG(LogTemp, Log, TEXT("Dig Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABaseCharacter::ReceiveBall()
{
	UE_LOG(LogTemp, Log, TEXT("Receive Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = Company->GetActorLocation();

	Ball->ReceiveMovement(1.2, StartPos, EndPos, EBallState::eStableSetted);

	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABaseCharacter::BlockBall()
{
	UE_LOG(LogTemp, Log, TEXT("Block Ball"));
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABaseCharacter::TossBall()
{
	UE_LOG(LogTemp, Log, TEXT("Toss Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABaseCharacter::PassBall()
{
	UE_LOG(LogTemp, Log, TEXT("Pass Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABaseCharacter::SpikeBall()
{
	UE_LOG(LogTemp, Log, TEXT("Spike Ball"));
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABaseCharacter::FloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Floating Ball"));
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

bool ABaseCharacter::JudgePassMode()
{
	// 1. Set Direction
	if (!Company)
		return false;

	Direction = GetDirectionFromPlayer(Company->GetActorLocation());

	// 2. Check If Toss is Possible
	FName FilterName = Direction == FName("Back") ? FName("Front") : Direction;
	float RequiredHeight = GetRequiredHeightFromOffset(TossOffsetMap, FilterName);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		OffenceMode = EOffenceMode::OM_TOSS;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 3. Check If Pass is Possible
	RequiredHeight = GetRequiredHeightFromOffset(PassOffsetMap, Direction);
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		OffenceMode = EOffenceMode::OM_PASS;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	return false;
}

bool ABaseCharacter::JudgeAttackMode()
{
	// 1. Set Spike
	SpikeMode = FName("FullSpike");

	// 2. Check If Spike is Possible
	float RequiredHeight = GetRequiredHeightFromOffset(SpikeOffsetMap, SpikeMode);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		OffenceMode = EOffenceMode::OM_SPIKE;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 3. Set Direction to Enemy's Court: 
	Direction = FName("Front");

	// 4. Check If Floating is Possible
	RequiredHeight = GetRequiredHeightFromOffset(FloatingOffsetMap, Direction);
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		OffenceMode = EOffenceMode::OM_FLOATING;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	return false;
}

bool ABaseCharacter::JudgeReceiveMode()
{
	// 1. Set Direction
	if (!Company)
		return false;

	Direction = GetDirectionFromPlayer(Company->GetActorLocation());

	// 2. Check If Receive is Possible
	float RequiredHeight = GetRequiredHeightFromOffset(ReceiveOffsetMap, Direction);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		DefenceMode = EDefenceMode::DM_RECEIVE;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 3. Check If Dig is Possible
	RequiredHeight = GetRequiredHeightFromOffset(DigOffsetMap, FName("Front"));
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	if (DropInfo.remain_time > 0.0f)
	{
		Direction = FName("Front");
		DefenceMode = EDefenceMode::DM_DIG;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	return false;
}

bool ABaseCharacter::JudgeBlockMode()
{
	DefenceMode = EDefenceMode::DM_DIG;
	Direction = FName("Front");

	return true;
}

void ABaseCharacter::SetServiceMode()
{
	// Set Service Mode& Calculate Ball's Action Values
	// 1. Set Service Mode upon Player Pos : TODO 
	// 2. Call Ball's Calculate values : TODO
	JudgeServiceMode();

	TimingMax = RemainingTimeToAction;

	// 3. Play Service Anim
	PlayServiceAnimation();
}

void ABaseCharacter::SetPassMode()
{
	if (!JudgePassMode())
		return;

	TimingMax = RemainingTimeToAction;

	PlayPassAnimation();
}

void ABaseCharacter::SetAttackMode()
{
	if (!JudgeAttackMode())
		return;

	TimingMax = RemainingTimeToAction;

	PlayAttackAnimation();
}

void ABaseCharacter::SetReceiveMode()
{
	if (!JudgeReceiveMode())
		return;

	TimingMax = RemainingTimeToAction;

	PlayReceiveAnimation();
}

void ABaseCharacter::SetBlockMode()
{
	if (!JudgeBlockMode())
		return;

	PlayBlockAnimation();
}

FString ABaseCharacter::GetPlayerMode()
{
	TArray<FString> player_modes =
	{
		"ReadyStable",
		"ReadyOffensive"
	};

	if (PlayerTurn == EPlayerTurn::PT_SERVICE)
	{
		return player_modes[1];
	}

	if (PlayerTurn == EPlayerTurn::PT_DEFENCE)
	{
		return player_modes[0];
	}
	if (PlayerTurn == EPlayerTurn::PT_OFFENCE)
	{
		switch (OffenceMode)
		{
		case EOffenceMode::OM_SPIKE :
			return player_modes[1];
		default:
			return player_modes[0];
		}
	}
	return "";
}

void ABaseCharacter::PlayServiceAnimation()
{
	MontageStarted();
	float PlayRate = CalculatePlayRate(RemainingTimeToAction, ServiceMontage, ServiceMode);
	PlayAnimMontage(ServiceMontage, PlayRate, ServiceMode);
}

void ABaseCharacter::PlayPassAnimation()
{
	MontageStarted();
	float PlayRate;
	switch (OffenceMode)
	{
	case EOffenceMode::OM_TOSS:
		// Move To Action Pos
		//MoveToActionPos(*TossOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, TossMontage, Direction);
		PlayAnimMontage(TossMontage, PlayRate, Direction);
		break;
	case EOffenceMode::OM_PASS:
		// Move To Action Pos
		MoveToActionPos(*PassOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, PassMontage, Direction);
		PlayAnimMontage(PassMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayAttackAnimation()
{
	MontageStarted();
	float PlayRate;
	switch (OffenceMode)
	{
	case EOffenceMode::OM_SPIKE:
		// Move To Action Pos
		//MoveToActionPos(*SpikeOffsetMap.Find(SpikeMode));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, SpikeMontage, SpikeMode);
		PlayAnimMontage(SpikeMontage, PlayRate, SpikeMode);
		break;
	case EOffenceMode::OM_FLOATING:
		// Move To Action Pos
		//MoveToActionPos(*FloatingOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, FloatingMontage, Direction);
		PlayAnimMontage(FloatingMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayReceiveAnimation()
{
	MontageStarted();
	float PlayRate;
	switch (DefenceMode)
	{
	case EDefenceMode::DM_DIG:
		// Move To Action Pos
		//MoveToActionPos(*DigOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, DigMontage, Direction);
		PlayAnimMontage(DigMontage, PlayRate, Direction);
		break;
	case EDefenceMode::DM_RECEIVE:
		// Move To Action Pos
		MoveToActionPos(*ReceiveOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, ReceiveMontage, Direction);
		PlayAnimMontage(ReceiveMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayBlockAnimation()
{
	MontageStarted();
	PlayAnimMontage(BlockMontage, 1.0f, Direction);
}


float ABaseCharacter::GetMontageSectionLength(UAnimMontage* Montage, FName SectionName)
{
	float StartTime, EndTime;

	int32 SectionIndex = Montage->GetSectionIndex(SectionName);
	Montage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);

	return EndTime - StartTime;
}

float ABaseCharacter::GetRequiredHeightFromOffset(const TMap<FName, FVector>& Map, FName name)
{
	FVector ActorLocation = GetActorLocation();
	FVector Offset = *Map.Find(name);

	return ActorLocation.Z + Offset.Z;
}

FName ABaseCharacter::GetDirectionFromPlayer(FVector TargetPos)
{
	FVector ActorLocation = GetActorLocation();

	FVector DirVec = TargetPos - ActorLocation;
	DirVec.Z = 0;
	DirVec.Normalize();

	double DotResult = GetActorForwardVector().Dot(DirVec);

	if (DotResult > 0.3)
	{
		return FName("Front");
	}
	if (DotResult < -0.3)
	{
		return FName("Back");
	}

	FVector CrossVector = GetActorForwardVector().Cross(DirVec);
	DotResult = CrossVector.Dot(GetActorUpVector());

	if (DotResult > 0.0)
	{
		return FName("Right");
	}

	return FName("Left");
}

float ABaseCharacter::CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName)
{
	float StartTime, EndTime;

	int32 SectionIndex = Montage->GetSectionIndex(SectionName);
	Montage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);
	float AnimTime = EndTime - StartTime;

	return AnimTime / TimeRemaining;
}

FVector ABaseCharacter::RotateOffsetToCurrentDirection(FVector Vector)
{
	FVector ForwardVector = GetActorForwardVector();
	FVector StandardVector = FVector(1, 0, 0);

	auto quat = FQuat::FindBetweenVectors(StandardVector, ForwardVector);
	Vector = quat.RotateVector(Vector);
	return Vector;
}

void ABaseCharacter::MoveToActionPos(FVector Offset)
{
	Offset = RotateOffsetToCurrentDirection(Offset);

	// Set Destination to Action
	FVector Destination = ActionPos - Offset;

	// TODO : Set Speed To Destination
	SetActorLocation(Destination);
}
