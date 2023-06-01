// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Ball/Ball.h"
#include "World/VolleyballArenaBase.h"
#include "World/VolleyballGame.h"
#include "World/VolleyballTeam.h"
#include "GameFramework/Controller.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


AVolleyBallTeam* ABaseCharacter::GetEnemyTeam()
{
	if (!arena_)
		return nullptr;

	switch (GetMyTeam()->GetCourtName())
	{
	case ECourtName::eBeachSideTeam:
		return arena_->game_playing_->GetCourtTeam(ECourtName::eReefSideTeam);
		break;
	case ECourtName::eReefSideTeam:
		return arena_->game_playing_->GetCourtTeam(ECourtName::eBeachSideTeam);
		break;
	}

	return nullptr;
}

void ABaseCharacter::MontageStarted()
{
	is_montage_started_ = true;
	is_montage_ended_ = false;
}

void ABaseCharacter::MontageEnded()
{
	is_montage_ended_ = true;
	OffenceMode = EOffenceMode::OM_NONE;
	DefenceMode = EDefenceMode::DM_NONE;
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
	SetBlockHand();
	LoadDataTable();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	BlockCapsuleR->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), FName("hand_r"));
	BlockCapsuleL->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), FName("hand_l"));

	bUseControllerRotationYaw = false;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmoothingWalkRun(DeltaTime);

	MoveToOffsetDestination(DeltaTime);
}

void ABaseCharacter::SmoothingWalkRun(float DeltaTime)
{
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

void ABaseCharacter::MoveToOffsetDestination(float DeltaTime)
{
	if (!bIsMoveToOffset)
		return;

	OffsetTimer += DeltaTime;
	
	FVector Offset = OffsetDestination - OffsetStart;
	FVector LocationThisTime = OffsetStart + Offset * OffsetTimer / RemainingTimeToAction;

	SetActorLocation(LocationThisTime, true);
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
	WalkSpeed = 500.0f;
	SprintSpeed = 700.0f;

	OffenceMode = EOffenceMode::OM_NONE;
	DefenceMode = EDefenceMode::DM_NONE;

	Direction = FName(TEXT("Left"));

	bIsClicking = false;
	bIsSprint = false; 
	bIsMoveToOffset = false;

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

void ABaseCharacter::SetBlockHand()
{
	BlockCapsuleR = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockCapsuleR"));
	BlockCapsuleR->SetCapsuleHalfHeight(44.0f);
	BlockCapsuleR->SetCapsuleRadius(22.0f);
	BlockCapsuleR->SetHiddenInGame(false);
	
	BlockCapsuleL = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockCapsuleL"));
	BlockCapsuleL->SetCapsuleHalfHeight(44.0f);
	BlockCapsuleL->SetCapsuleRadius(22.0f);
	BlockCapsuleL->SetHiddenInGame(false);
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
		FVector EndPos = dest_turnover_to_;
		EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

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
		Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
	}
}

void ABaseCharacter::DigBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Dig Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = Company->GetActorLocation();
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->DigMovement(1.2, StartPos, EndPos, EBallState::eStableSetted);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}

void ABaseCharacter::ReceiveBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Receive Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = Company->GetActorLocation();
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->ReceiveMovement(1.2, StartPos, EndPos, EBallState::eStableSetted);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}

void ABaseCharacter::BlockBall()
{
	UE_LOG(LogTemp, Log, TEXT("Block Ball"));
}

void ABaseCharacter::TossBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Toss Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos;

	EndPos = Company->dest_position_;

	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);
	Ball->TossMovement(1.2, StartPos, EndPos, EBallState::eStableSetted);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}

void ABaseCharacter::PassBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Pass Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos;
	if (FVector::PointsAreSame(Company->dest_position_, FVector(0, 0, 0)))
	{
		EndPos = Company->GetActorLocation();
	}
	else
	{
		EndPos = Company->dest_position_;
	}

	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);
	Ball->TossMovement(1.2, StartPos, EndPos, EBallState::eStableSetted);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}

void ABaseCharacter::SpikeBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Spike Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = dest_turnover_to_;
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->SpikeMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}

void ABaseCharacter::FloatingBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Floating Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = dest_turnover_to_;
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->FloatingMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());
}


bool ABaseCharacter::JudgeServiceMode()
{
	if (GetMyTeam()->IsVectorInTeamBox(GetActorLocation()))
		return false;

	RemainingTimeToAction = ServiceMontage->GetSectionLength(ServiceMontage->GetSectionIndex(ServiceMode));
	return true;
}

bool ABaseCharacter::JudgePassMode()
{
	if (!bIsInBallTrigger || !Company)
		return false;

	float TimeToPlayAnim = 0;

	// 1. Set Direction
	Direction = GetDirectionFromPlayer(Company->GetActorLocation());

	// 2. Check If Toss is Possible
	FName FilterName = Direction.Compare(FName("Back")) == 0 ? FName("Front") : Direction;
	float RequiredHeight = GetRequiredHeightFromOffset(TossOffsetMap, FilterName);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = TossMontage->GetSectionLength(TossMontage->GetSectionIndex(FilterName));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
	{
		Direction = FilterName;
		OffenceMode = EOffenceMode::OM_TOSS;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 4. Check If Pass is Possible
	RequiredHeight = GetRequiredHeightFromOffset(PassOffsetMap, Direction);
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 5. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = PassMontage->GetSectionLength(PassMontage->GetSectionIndex(Direction));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
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
	if (!bIsInBallTrigger || !Company)
		return false;

	float TimeToPlayAnim = 0;

	// 1. Set Spike
	SpikeMode = FName("FullSpike");

	// 2. Check If Spike is Possible
	float RequiredHeight = GetRequiredHeightFromOffset(SpikeOffsetMap, SpikeMode);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = SpikeMontage->GetSectionLength(SpikeMontage->GetSectionIndex(SpikeMode));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
	{
		OffenceMode = EOffenceMode::OM_SPIKE;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 4. Set Direction to Enemy's Court : TODO
	Direction = FName("Front");

	// 5. Check If Floating is Possible
	RequiredHeight = GetRequiredHeightFromOffset(FloatingOffsetMap, Direction);
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 6. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = FloatingMontage->GetSectionLength(FloatingMontage->GetSectionIndex(Direction));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
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
	if (!bIsInBallTrigger || !Company)
		return false;

	float TimeToPlayAnim = 0;

	// 1. Set Direction
	Direction = GetDirectionFromPlayer(Company->GetActorLocation());

	// 2. Check If Receive is Possible
	float RequiredHeight = GetRequiredHeightFromOffset(ReceiveOffsetMap, Direction);
	auto DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = ReceiveMontage->GetSectionLength(ReceiveMontage->GetSectionIndex(Direction));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
	{
		DefenceMode = EDefenceMode::DM_RECEIVE;
		RemainingTimeToAction = DropInfo.remain_time;
		ActionPos = DropInfo.drop_pos;
		return true;
	}

	// 4. Check If Dig is Possible
	RequiredHeight = GetRequiredHeightFromOffset(DigOffsetMap, FName("Front"));
	DropInfo = Ball->GetDropInfo(RequiredHeight);

	// 5. If Remaining Time to Arrive is more than AnimTime, Waiting
	TimeToPlayAnim = DigMontage->GetSectionLength(DigMontage->GetSectionIndex(Direction));

	if (DropInfo.remain_time > TimeToPlayAnim)
		return false;

	if (DropInfo.remain_time > 0.0f)//TimeToPlayAnim * 0.4f)
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
	DefenceMode = EDefenceMode::DM_BLOCK;
	Direction = FName("Front");

	return true;
}

void ABaseCharacter::SetServiceMode()
{
	// Set Service Mode& Calculate Ball's Action Values
	// 1. Set Service Mode upon Player Pos : TODO 
	// 2. Call Ball's Calculate values : TODO
	if (!JudgeServiceMode())
		return;

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
		SetMoveToActionPos(*TossOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, TossMontage, Direction);
		PlayAnimMontage(TossMontage, PlayRate, Direction);
		break;
	case EOffenceMode::OM_PASS:
		// Move To Action Pos
		SetMoveToActionPos(*PassOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, PassMontage, Direction);
		PlayAnimMontage(PassMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayAttackAnimation()
{
	MontageStarted();
	float PlayRate;
	FVector RotationDir;
	FQuat quat;
	switch (OffenceMode)
	{
	case EOffenceMode::OM_SPIKE:
		// Move To Action Pos
		SetMoveToActionPos(*SpikeOffsetMap.Find(SpikeMode));
		// Rotate To End Pos
		RotationDir = (dest_turnover_to_ - GetActorLocation());
		RotationDir.Z = 0;
		RotationDir.Normalize();
		quat = FQuat::FindBetweenVectors(GetActorForwardVector(), RotationDir);
		AddActorLocalRotation(quat);

		PlayRate = CalculatePlayRate(RemainingTimeToAction, SpikeMontage, SpikeMode);
		PlayAnimMontage(SpikeMontage, PlayRate, SpikeMode);
		break;
	case EOffenceMode::OM_FLOATING:
		// Move To Action Pos
		SetMoveToActionPos(*FloatingOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, FloatingMontage, Direction);
		PlayAnimMontage(FloatingMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayReceiveAnimation()
{
	MontageStarted();
	float PlayRate;
	FVector RotationDir;
	FQuat quat;
	switch (DefenceMode)
	{
	case EDefenceMode::DM_DIG:
		// Move To Action Pos
		SetMoveToActionPos(*DigOffsetMap.Find(Direction));
		RotationDir = (ActionPos - GetActorLocation());
		RotationDir.Z = 0;
		RotationDir.Normalize();
		quat = FQuat::FindBetweenVectors(GetActorForwardVector(), RotationDir);
		AddActorLocalRotation(quat);

		PlayRate = CalculatePlayRate(RemainingTimeToAction, DigMontage, Direction);
		PlayAnimMontage(DigMontage, PlayRate, Direction);
		break;
	case EDefenceMode::DM_RECEIVE:
		// Move To Action Pos
		SetMoveToActionPos(*ReceiveOffsetMap.Find(Direction));
		PlayRate = CalculatePlayRate(RemainingTimeToAction, ReceiveMontage, Direction);
		PlayAnimMontage(ReceiveMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayBlockAnimation()
{
	MontageStarted();

	auto RotationDir = GetMyTeam()->GetCourtName() == ECourtName::eReefSideTeam ? FVector(0, 1, 0) : FVector(0, -1, 0);
	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), RotationDir);
	AddActorLocalRotation(quat);

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

void ABaseCharacter::SetMoveToActionPos(FVector Offset)
{
	Offset = RotateOffsetToCurrentDirection(Offset);

	// Set Start Pos From current pos
	OffsetStart = GetActorLocation();

	// Set Destination to Action
	OffsetDestination = ActionPos - Offset;

	bIsMoveToOffset = true;
}

FVector ABaseCharacter::GetRandomPosInRange(const FVector& Center, float accuracy)
{
	if (accuracy >= 1.1f) {
		accuracy = 0.0f;
	}
	float radius = 300.0f * (1.0f - accuracy);
	
	FVector position;
	position.X = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
	position.Y = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

	position.Normalize();
	position *= radius;

	position += Center;

	return position;
}

