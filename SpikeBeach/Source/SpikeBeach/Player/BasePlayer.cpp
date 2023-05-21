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

	if (bIsGauging)
	{
		Gauge = Gauge < 100.0f ? Gauge + DeltaTime * GaugeSpeed : 100.0f;
		UE_LOG(LogTemp, Log, TEXT("Gauge : %f"), Gauge);
	}
	else
	{
		Gauge = 0;
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
	if (!bIsGauging)
	{
		bIsGauging = true;
		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_SERVICE:
			state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_StableType);
			CheckServiceMode();
			break;
		case EPlayerTurn::PT_DEFENCE:
			DefenceMode = CheckReceiveMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			OffenceMode = CheckAttackMode();
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
		PlayServiceAnimation();
		break;
	case EPlayerTurn::PT_DEFENCE:
		PlayReceiveAnimation();
		break;
	case EPlayerTurn::PT_OFFENCE:
		PlayAttackAnimation();
		break;
	}

	bIsGauging = false;
}

void ABasePlayer::RClickTriggered(const FInputActionValue& Value)
{
	if (!bIsGauging)
	{
		bIsGauging = true;
		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_DEFENCE:
			DefenceMode = CheckBlockMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			OffenceMode = CheckPassMode();
			DefenceMode = EDefenceMode::DM_NONE;
			break;
		}
	}

}

void ABasePlayer::RClickCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("RClick Complete"));

	switch (PlayerTurn)
	{
	case EPlayerTurn::PT_DEFENCE:
		PlayBlockAnimation();
		break;
	case EPlayerTurn::PT_OFFENCE:
		PlayPassAnimation();
		break;
	}

	bIsGauging = false;
}

void ABasePlayer::SprintTriggered(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Triggered"));

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasePlayer::SprintCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Sprint Completed"));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasePlayer::ServiceFloatingBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Float Ball"));
}

void ABasePlayer::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));
	state_ui_notices_.Enqueue(EStateUINotice::eUnshowedGauge_StableType);

	//PlayerTurn = EPlayerTurn::PT_DEFENCE;
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

void ABasePlayer::CheckServiceMode()
{
	ServiceMode = FName(TEXT("Jump"));
}

EOffenceMode ABasePlayer::CheckPassMode()
{
	// Mode Check
	int32 mode = FMath::RandRange((int32)EOffenceMode::OM_TOSS, (int32)EOffenceMode::OM_PASS);

	// Direction Check
	// TOSS : Front(Back) / Left / Right
	// PASS : Front / Back / Right / Left
	Direction = FName(TEXT("Front"));

	return EOffenceMode(mode);
}

EOffenceMode ABasePlayer::CheckAttackMode()
{
	// Mode Check
	int32 mode = FMath::RandRange((int32)EOffenceMode::OM_SPIKE, (int32)EOffenceMode::OM_FLOATING);

	// Direction Check
	// Floating : Front(Back) / Left / Right
	Direction = FName(TEXT("Front"));

	return EOffenceMode(mode);
}

EDefenceMode ABasePlayer::CheckReceiveMode()
{
	// Mode Check
	int32 mode = FMath::RandRange((int32)EDefenceMode::DM_DIG, (int32)EDefenceMode::DM_RECEIVE);

	// Direction Check
	// Dig : Front / Left / Right
	// Receive : Front / Back / Right / Left
	Direction = FName(TEXT("Front"));

	return EDefenceMode(mode);
}

EDefenceMode ABasePlayer::CheckBlockMode()
{
	// Direction Check
	// Block : Front / Left / Right
	Direction = FName(TEXT("Front"));

	return EDefenceMode::DM_BLOCK;
}

void ABasePlayer::PlayServiceAnimation()
{
	FName Service = FName(TEXT("Spoon"));
	PlayAnimMontage(ServiceMontage, 1.0f, ServiceMode);
}

void ABasePlayer::PlayPassAnimation()
{
	switch (OffenceMode)
	{
	case EOffenceMode::OM_TOSS:
		PlayAnimMontage(TossMontage, 1.0f, Direction);
		break;
	case EOffenceMode::OM_PASS:
		PlayAnimMontage(PassMontage, 1.0f, Direction);
		break;
	}
}

void ABasePlayer::PlayAttackAnimation()
{
	FName Spike = FName(TEXT("FullSpike"));

	switch (OffenceMode)
	{
	case EOffenceMode::OM_SPIKE:
		PlayAnimMontage(SpikeMontage, 1.0f, Spike);
		break;
	case EOffenceMode::OM_FLOATING:
		PlayAnimMontage(FloatingMontage, 1.0f, Direction);
		break;
	}
}

void ABasePlayer::PlayReceiveAnimation()
{
	switch (DefenceMode)
	{
	case EDefenceMode::DM_DIG:
		PlayAnimMontage(DigMontage, 1.0f, Direction);
		break;
	case EDefenceMode::DM_RECEIVE:
		PlayAnimMontage(ReceiveMontage, 1.0f, Direction);
		break;
	}
}

void ABasePlayer::PlayBlockAnimation()
{
	PlayAnimMontage(BlockMontage, 1.0f, Direction);
}



