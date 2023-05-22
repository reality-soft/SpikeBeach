// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "CustomPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABasePlayer::ABasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetSuperSettings();

	SetPlayerAttributes();

	SetCapsuleComponent();

	SetCharacterMovement();

	SetCamera();

	SetInputAction();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = false;

	//Add Input Mapping Context
	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
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
	if (!bIsSprint && fabs(GetCharacterMovement()->GetMaxSpeed() - WalkSpeed) > FLT_EPSILON)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed - 300.0f * DeltaTime;
	}
	else if (!bIsSprint && fabs(GetCharacterMovement()->GetMaxSpeed() - WalkSpeed) <= FLT_EPSILON)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Clicking
		EnhancedInputComponent->BindAction(LClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::LClickTriggered);
		EnhancedInputComponent->BindAction(LClickAction, ETriggerEvent::Completed, this, &ABasePlayer::LClickCompleted);
		EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::RClickTriggered);
		EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Completed, this, &ABasePlayer::RClickCompleted);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABasePlayer::SprintTriggered);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayer::SprintCompleted);
	}

}

void ABasePlayer::SetSuperSettings()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ABasePlayer::SetPlayerAttributes()
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

void ABasePlayer::SetCapsuleComponent()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.0f);
	GetCapsuleComponent()->SetRelativeLocation({ 0.f, 0.f, -90.f });
}

void ABasePlayer::SetCharacterMovement()
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

void ABasePlayer::SetCamera()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeRotation({ -10.f, 0.f, 0.f });
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ABasePlayer::SetInputAction()
{
	//ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT(TEXT("/Game/Content/Character/Input/IMC_Player.IMC_Player"));
	//if (DEFAULT_CONTEXT.Succeeded())
	//{
	//	DefaultMappingContext = DEFAULT_CONTEXT.Object;
	//}
	//
	//ConstructorHelpers::FObjectFinder<UInputAction>MOVE_ACTION(TEXT("/Game/Content/Character/Input/Action/IA_Move.IA_Move"));
	//if (MOVE_ACTION.Succeeded())
	//{
	//	MoveAction = MOVE_ACTION.Object;
	//}
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasePlayer::LClickTriggered(const FInputActionValue& Value)
{
	if (!bIsClicking)
	{
		bIsClicking = true;

		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_SERVICE:
			state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_StableType);
			SetServiceMode();
			break;
		case EPlayerTurn::PT_DEFENCE:
			SetReceiveMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			SetAttackMode();
			DefenceMode = EDefenceMode::DM_NONE;
			break;
		}
	}
}

void ABasePlayer::LClickCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("LClick Complete"));

	switch (PlayerTurn)
	{
	case EPlayerTurn::PT_SERVICE:
		state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_StableType);
		break;
	}

	TimingAccuracy = TimingTimer / TimingMax;
	UE_LOG(LogTemp, Log, TEXT("TimingAccuracy : %f"), TimingAccuracy);
	UE_LOG(LogTemp, Log, TEXT("TimingTimer : %f"), TimingTimer);
}

void ABasePlayer::RClickTriggered(const FInputActionValue& Value)
{
	if (!bIsClicking)
	{
		bIsClicking = true;

		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_DEFENCE:
			SetBlockMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			SetPassMode();
			DefenceMode = EDefenceMode::DM_NONE;
			break;
		}
	}

}

void ABasePlayer::RClickCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("RClick Complete"));

	bIsClicking = false;

	TimingAccuracy = TimingTimer / TimingMax;
	UE_LOG(LogTemp, Log, TEXT("TimingAccuracy : %f"), TimingAccuracy);
	UE_LOG(LogTemp, Log, TEXT("TimingTimer : %f"), TimingTimer);
}

void ABasePlayer::SprintTriggered(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Triggered"));
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasePlayer::SprintCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Completed"));

	bIsSprint = false;

	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasePlayer::ServiceFloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Float Ball"));
}

void ABasePlayer::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));
	state_ui_notices_.Enqueue(EStateUINotice::eUnshowedGauge_StableType);

	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABasePlayer::DigBall()
{
	UE_LOG(LogTemp, Log, TEXT("Dig Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABasePlayer::ReceiveBall()
{
	UE_LOG(LogTemp, Log, TEXT("Receive Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABasePlayer::BlockBall()
{
	UE_LOG(LogTemp, Log, TEXT("Block Ball"));
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABasePlayer::TossBall()
{
	UE_LOG(LogTemp, Log, TEXT("Toss Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABasePlayer::PassBall()
{
	UE_LOG(LogTemp, Log, TEXT("Pass Ball"));
	PlayerTurn = EPlayerTurn::PT_OFFENCE;
}

void ABasePlayer::SpikeBall()
{
	UE_LOG(LogTemp, Log, TEXT("Spike Ball"));

	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABasePlayer::FloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Floating Ball"));
	PlayerTurn = EPlayerTurn::PT_DEFENCE;
}

void ABasePlayer::SetServiceMode()
{
	// Set Service Mode& Calculate Ball's Action Values
	// 1. Set Service Mode upon Player Pos : TODO 
	// 2. Call Ball's Calculate values : TODO
	// 3. Call Ball's Calculate values : TODO
	JudgeServiceMode();

	TimingMax = RemainingTimeToAction;

	// 3. Play Service Anim
	PlayServiceAnimation();
}

void ABasePlayer::SetPassMode()
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

void ABasePlayer::SetAttackMode()
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

void ABasePlayer::SetReceiveMode()
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

void ABasePlayer::SetBlockMode()
{
	// Set Block Mode & Direction
	// 1. Mode Set
	// 2. Direction Check : TODO 
		// Block : Front / Left / Right
	JudgeBlockMode();

	// 2. Play Block Anim with 1.0 rate
	PlayBlockAnimation();
}

void ABasePlayer::PlayServiceAnimation()
{
	float PlayRate = CalculatePlayRate(RemainingTimeToAction, ServiceMontage, ServiceMode);

	PlayAnimMontage(ServiceMontage, PlayRate, ServiceMode);
}

void ABasePlayer::PlayPassAnimation()
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

void ABasePlayer::PlayAttackAnimation()
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

void ABasePlayer::PlayReceiveAnimation()
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

void ABasePlayer::PlayBlockAnimation()
{
	PlayAnimMontage(BlockMontage, 1.0f, Direction);
}


float ABasePlayer::CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName)
{
	float StartTime, EndTime;

	int32 SectionIndex = Montage->GetSectionIndex(SectionName);
	Montage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);
	float AnimTime = EndTime - StartTime;

	return AnimTime / TimeRemaining;
}


