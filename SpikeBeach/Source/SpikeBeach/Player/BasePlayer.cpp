// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "../World/VolleyballArenaBase.h"
#include "../World/VolleyballTeam.h"
#include "../World/VolleyballGame.h"
#include "CustomPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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
	SetInputAction();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();


	//Add Input Mapping Context
	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_LClick_To_Service);
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

		// Ball Cursor
		EnhancedInputComponent->BindAction(BallCursorControl, ETriggerEvent::Triggered, this, &ABasePlayer::BallCursorTriggered);
	}

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
			state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_OffensiveRG);
			SetServiceMode();
			break;
		case EPlayerTurn::PT_DEFENCE:
			state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_StableRG);
			SetReceiveMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_OffensiveRG);
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

	if (1.2f > TimingAccuracy && TimingAccuracy > 0.8f)
	{

	}

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
			state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_OffensiveRG);
			SetBlockMode();
			OffenceMode = EOffenceMode::OM_NONE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_StableRG);
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

void ABasePlayer::BallCursorTriggered(const FInputActionValue& Value)
{
	if (CanControlBallCursor == false)
		return;

	ball_cursor_value_.X += Value.Get<FInputActionValue::Axis2D>().X * 0.01f;
	ball_cursor_value_.Y += Value.Get<FInputActionValue::Axis2D>().Y * 0.01f;

	ball_cursor_value_.X = std::min(ball_cursor_value_.X,  1.0);
	ball_cursor_value_.X = std::max(ball_cursor_value_.X, -1.0);
												 
	ball_cursor_value_.Y = std::min(ball_cursor_value_.Y,  1.0);
	ball_cursor_value_.Y = std::max(ball_cursor_value_.Y, -1.0);

	ECourtName agains_team;

	switch (GetMyTeam()->GetCourtName())
	{
	case ECourtName::eReefSideTeam:
		agains_team = ECourtName::eBeachSideTeam;
		break;
	case ECourtName::eBeachSideTeam:
		agains_team = ECourtName::eReefSideTeam;
		break;
	}

	arena_->game_playing_->GetCourtTeam(agains_team)->UpdateBallCursor(ball_cursor_value_);
}

void ABasePlayer::MontageEnded()
{
	is_montage_started_ = false;
	is_montage_ended_ = true;
}


void ABasePlayer::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));

	if (Ball)
	{
		FVector StartPos = Ball->GetActorLocation();
		FVector EndPos = GetEnemyTeam()->ball_cursor_->GetComponentLocation();

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

	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::ReceiveBall() 
{
	ABaseCharacter::ReceiveBall();
	
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::DigBall() 
{
	ABaseCharacter::DigBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::BlockBall()
{
	ABaseCharacter::BlockBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::TossBall()
{
	ABaseCharacter::TossBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);

}

void ABasePlayer::PassBall()
{
	ABaseCharacter::PassBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::SpikeBall()
{
	ABaseCharacter::SpikeBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::FloatingBall()
{
	ABaseCharacter::FloatingBall();
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::PlayServiceAnimation()
{
	CanControlBallCursor = false;
	ABaseCharacter::PlayServiceAnimation();
}

void ABasePlayer::PlayPassAnimation()
{
	ABaseCharacter::PlayPassAnimation();
}

void ABasePlayer::PlayAttackAnimation()
{
	ABaseCharacter::PlayAttackAnimation();

}

void ABasePlayer::PlayReceiveAnimation()
{
	ABaseCharacter::PlayReceiveAnimation();
}

void ABasePlayer::PlayBlockAnimation()
{
	ABaseCharacter::PlayBlockAnimation();
}


void ABasePlayer::ClearBallCursor()
{
	ball_cursor_value_.X = 0;
	ball_cursor_value_.Y = 0;
}
