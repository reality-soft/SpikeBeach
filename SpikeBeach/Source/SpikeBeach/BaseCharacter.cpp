// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/Controller.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetSuperSettings();
	SetPlayerAttributes();
	SetCapsuleComponent();
	SetCharacterMovement();
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

void ABaseCharacter::ServiceFloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Float Ball"));
}

void ABaseCharacter::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));

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
	// Set Pass Mode & Direction Check & Calculate Ball's Action Values
	// 1. Mode Check
		// Set Pass Mode upon Ball's Z coord
	// 2. Direction Check
		// Set Direction upon Team's Position
		// TOSS : Front(Back) / Left / Right
		// PASS : Front / Back / Right / Left
	// 3. Call Ball's Calculate values : TODO
	JudgePassMode();

	TimingMax = RemainingTimeToAction;

	// 4. Play Pass Anim
	PlayPassAnimation();
}

void ABaseCharacter::SetAttackMode()
{
	// Set Attack Mode & Direction & Calculate Ball's Action Values
	// 1. Mode Check : TODO 
		// Set Attack Mode upon Ball's Z coord
	// 2. Direction Check : TODO 
		// Floating : Front(Back) / Left / Right
	// 3. Set SpikeMode if Spike
		// FullSpike / SemiSpike
	// 4. Call Ball's Calculate values : TODO
	JudgeAttackMode();

	TimingMax = RemainingTimeToAction;

	// 5. Play Attack Anim
	PlayAttackAnimation();
}

void ABaseCharacter::SetReceiveMode()
{
	// Set Receive Mode & Direction & Calculate Ball's Action Values
	// 1. Mode Check : TODO 
		// Set Receive Mode upon Ball's Z coord
	// 2. Direction Check : TODO 
		// Dig : Front / Left / Right
		// Receive : Front / Back / Right / Left
	// 3. Call Ball's Calculate values
	JudgeReceiveMode();

	TimingMax = RemainingTimeToAction;

	// 4. Play Receive Anim
	PlayReceiveAnimation();
}

void ABaseCharacter::SetBlockMode()
{
	// Set Block Mode & Direction
	// 1. Mode Set
	// 2. Direction Check : TODO 
		// Block : Front / Left / Right
	JudgeBlockMode();

	// 2. Play Block Anim with 1.0 rate
	PlayBlockAnimation();
}

void ABaseCharacter::PlayServiceAnimation()
{
	float PlayRate = CalculatePlayRate(RemainingTimeToAction, ServiceMontage, ServiceMode);

	PlayAnimMontage(ServiceMontage, PlayRate, ServiceMode);
}

void ABaseCharacter::PlayPassAnimation()
{
	float PlayRate;
	switch (OffenceMode)
	{
	case EOffenceMode::OM_TOSS:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, TossMontage, Direction);
		PlayAnimMontage(TossMontage, PlayRate, Direction);
		break;
	case EOffenceMode::OM_PASS:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, PassMontage, Direction);
		PlayAnimMontage(PassMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayAttackAnimation()
{
	float PlayRate;

	switch (OffenceMode)
	{
	case EOffenceMode::OM_SPIKE:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, SpikeMontage, SpikeMode);
		PlayAnimMontage(SpikeMontage, PlayRate, SpikeMode);
		break;
	case EOffenceMode::OM_FLOATING:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, FloatingMontage, Direction);
		PlayAnimMontage(FloatingMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayReceiveAnimation()
{
	float PlayRate;

	switch (DefenceMode)
	{
	case EDefenceMode::DM_DIG:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, DigMontage, Direction);
		PlayAnimMontage(DigMontage, PlayRate, Direction);
		break;
	case EDefenceMode::DM_RECEIVE:
		PlayRate = CalculatePlayRate(RemainingTimeToAction, ReceiveMontage, Direction);
		PlayAnimMontage(ReceiveMontage, PlayRate, Direction);
		break;
	}
}

void ABaseCharacter::PlayBlockAnimation()
{
	PlayAnimMontage(BlockMontage, 1.0f, Direction);
}


float ABaseCharacter::CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName)
{
	float StartTime, EndTime;

	int32 SectionIndex = Montage->GetSectionIndex(SectionName);
	Montage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);
	float AnimTime = EndTime - StartTime;

	return AnimTime / TimeRemaining;
}