// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "CustomPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ABasePlayer::ABasePlayer() : ABaseCharacter()
{
	//SetCamera();
	SetInputAction();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerTurn = EPlayerTurn::PT_DEFENCE;

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
	// If Moving to Action, Move Input disabled
	if (is_montage_started_)
		return;

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
	//UE_LOG(LogTemp, Log, TEXT("LClick Complete"));

	bIsClicking = false;

	TimingAccuracy = TimingTimer / TimingMax;
	//UE_LOG(LogTemp, Log, TEXT("TimingAccuracy : %f"), TimingAccuracy);
	//UE_LOG(LogTemp, Log, TEXT("TimingTimer : %f"), TimingTimer);
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
	//UE_LOG(LogTemp, Log, TEXT("RClick Complete"));

	bIsClicking = false;

	TimingAccuracy = TimingTimer / TimingMax;
	//UE_LOG(LogTemp, Log, TEXT("TimingAccuracy : %f"), TimingAccuracy);
	//UE_LOG(LogTemp, Log, TEXT("TimingTimer : %f"), TimingTimer);
}

void ABasePlayer::SprintTriggered(const FInputActionValue& Value)
{
	if (bIsSprint) return;

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

void ABasePlayer::MontageEnded()
{
	is_montage_started_ = false;
	is_montage_ended_ = true;
}


void ABasePlayer::ServiceHitBall()
{
	ABaseCharacter::ServiceHitBall();
	auto SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Offset (%f, %f, %f)"), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_OffensiveType);
}

void ABasePlayer::ReceiveBall() 
{
	ABaseCharacter::ReceiveBall();
	auto SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;
	
	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Receive %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_StableType);
}

void ABasePlayer::DigBall() 
{
	ABaseCharacter::DigBall();
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_StableType);
}

void ABasePlayer::BlockBall()
{
	ABaseCharacter::BlockBall();
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_OffensiveType);
}

void ABasePlayer::TossBall()
{
	ABaseCharacter::TossBall();
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_StableType);

}

void ABasePlayer::PassBall()
{
	ABaseCharacter::PassBall();
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightLowerArm")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Pass %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_StableType);
}

void ABasePlayer::SpikeBall()
{
	ABaseCharacter::SpikeBall();
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_OffensiveType);
}

void ABasePlayer::FloatingBall()
{
	ABaseCharacter::FloatingBall();
	//state_ui_notices_.Enqueue(EStateUINotice::eFinishedGauge_OffensiveType);
}

void ABasePlayer::PlayServiceAnimation()
{
	ABaseCharacter::PlayServiceAnimation();
	//state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_OffensiveType);
}

void ABasePlayer::PlayPassAnimation()
{
	ABaseCharacter::PlayPassAnimation();
	//state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_StableType);
}

void ABasePlayer::PlayAttackAnimation()
{
	ABaseCharacter::PlayAttackAnimation();
	//state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_OffensiveType);

}

void ABasePlayer::PlayReceiveAnimation()
{
	ABaseCharacter::PlayReceiveAnimation();
	//state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_StableType);
}

void ABasePlayer::PlayBlockAnimation()
{
	ABaseCharacter::PlayBlockAnimation();
	//state_ui_notices_.Enqueue(EStateUINotice::eStartedGauge_OffensiveType);
}


