// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "../World/VolleyballArenaBase.h"
#include "../World/VolleyballTeam.h"
#include "../World/VolleyballGame.h"
#include "CustomPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Quat.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABasePlayer::ABasePlayer() : ABaseCharacter()
{
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

	//state_ui_notices_.Enqueue(EStateUINotice::UI_);
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto player_controller = Cast<ACustomPlayerController>(Controller);
	//if (Company->GetPlayerRole() == EPlayerRole::PR_A_TOSS)
	//{
	//	player_controller->SetShowMouseCursor(true);
	//}
	//else
	//{
	//	player_controller->SetShowMouseCursor(false);
	//}
	auto player_controller = Cast<ACustomPlayerController>(Controller);
	player_controller->SetShowMouseCursor(false);

	if (CanControlBallCursor)
	{
		if (GetPlayerRole() == EPlayerRole::PR_S_SERVICE || GetPlayerRole() == EPlayerRole::PR_A_ATTACK)
		{
			dest_turnover_to_ = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

			// Rotate To End Pos
			auto RotationDir = (dest_turnover_to_ - GetActorLocation());
			RotationDir.Z = 0;
			RotationDir.Normalize();
			auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), RotationDir);

			AddActorLocalRotation(quat);
		}
	}

	TimingCalculateIfClick(DeltaTime);

	MouseTraceOnGround();
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

		// Wheel Click
		EnhancedInputComponent->BindAction(MouseWheelClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::WheelTriggered);


		// Ball Cursor
		EnhancedInputComponent->BindAction(BallCursorControl, ETriggerEvent::Triggered, this, &ABasePlayer::BallCursorTriggered);
	}

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

		if (GetMyTeam()->GetCourtName() == ECourtName::eBeachSideTeam)
		{
			MovementVector.X *= -1;
			MovementVector.Y *= -1;
		}

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
	}
	switch (PlayerTurn)
	{
	case EPlayerTurn::PT_SERVICE:
		CanControlBallCursor = true;
		SetServiceMode();
		break;
	case EPlayerTurn::PT_DEFENCE:
		CanControlBallCursor = false;
		SetReceiveMode();
		OffenceMode = EOffenceMode::OM_NONE;
		break;
	case EPlayerTurn::PT_OFFENCE:
		CanControlBallCursor = true;
		SetAttackMode();
		DefenceMode = EDefenceMode::DM_NONE;
		break;
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
	}
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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasePlayer::WheelTriggered(const FInputActionValue& Value)
{
	//if (Company->GetPlayerRole() != EPlayerRole::PR_A_TOSS)
	//	return;

	if (show_ping_cursor_ == false)
		return;

	if (traced_in_team_court_)
	{
		Company->ai_ping_order_.pass_ordered = true;
		Company->ai_ping_order_.pass_order_pos = current_traced_pos_;
		PingOrderEvent(EPingOrderType::ePassHere, current_traced_pos_);
	}
	else
	{
		PingOrderEvent(EPingOrderType::eWrongPos, current_traced_pos_);
	}	
}

void ABasePlayer::BallCursorTriggered(const FInputActionValue& Value)
{
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

	if (CanControlBallCursor == false)
	{
		ball_cursor_value_.X = 0.0f;
		ball_cursor_value_.Y = 0.0f;
		arena_->game_playing_->GetCourtTeam(agains_team)->ClearBallCursor();
		return;
	}

	ball_cursor_value_.X += Value.Get<FInputActionValue::Axis2D>().X * 0.01f;
	ball_cursor_value_.Y += Value.Get<FInputActionValue::Axis2D>().Y * 0.01f;

	ball_cursor_value_.X = std::min(ball_cursor_value_.X, 1.0);
	ball_cursor_value_.X = std::max(ball_cursor_value_.X, -1.0);

	ball_cursor_value_.Y = std::min(ball_cursor_value_.Y, 1.0);
	ball_cursor_value_.Y = std::max(ball_cursor_value_.Y, -1.0);

	arena_->game_playing_->GetCourtTeam(agains_team)->UpdateBallCursor(ball_cursor_value_);
}

void ABasePlayer::MontageEnded()
{
	is_montage_started_ = false;
	is_montage_ended_ = true;

	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}


void ABasePlayer::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));

	if (Ball)
	{
		FVector StartPos = Ball->GetActorLocation();
		FVector EndPos = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

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
	CanControlBallCursor = false;
	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}

void ABasePlayer::ReceiveBall() 
{
	ABaseCharacter::ReceiveBall();
}

void ABasePlayer::DigBall() 
{
	ABaseCharacter::DigBall();
}

void ABasePlayer::BlockBall()
{
	ABaseCharacter::BlockBall();
}

void ABasePlayer::TossBall()
{
	ABaseCharacter::TossBall();

}

void ABasePlayer::PassBall()
{
	ABaseCharacter::PassBall();
}

void ABasePlayer::SpikeBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Spike Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

	Ball->SpikeMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);

	CanControlBallCursor = false;
}

void ABasePlayer::FloatingBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Floating Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

	Ball->FloatingMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);

	CanControlBallCursor = false;
}

bool ABasePlayer::JudgeServiceMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgeServiceMode();
}

bool ABasePlayer::JudgePassMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgePassMode();
}

bool ABasePlayer::JudgeAttackMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgeAttackMode();
}

bool ABasePlayer::JudgeReceiveMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgeReceiveMode();
}

bool ABasePlayer::JudgeBlockMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgeBlockMode();
}

void ABasePlayer::PlayServiceAnimation()
{
	state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_OffensiveRG);

	//dest_turnover_to_ = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

	ABaseCharacter::PlayServiceAnimation();
}

void ABasePlayer::PlayPassAnimation()
{
	state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_StableRG);
	ABaseCharacter::PlayPassAnimation();
}

void ABasePlayer::PlayAttackAnimation()
{
	state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_OffensiveRG);

	dest_turnover_to_ = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

	ABaseCharacter::PlayAttackAnimation();

}

void ABasePlayer::PlayReceiveAnimation()
{
	state_ui_notices_.Enqueue(EStateUINotice::eActivateUI_StableRG);
	ABaseCharacter::PlayReceiveAnimation();
}

void ABasePlayer::PlayBlockAnimation()
{
	ABaseCharacter::PlayBlockAnimation();
}

void ABasePlayer::MouseTraceOnGround()
{
	//if (PlayerRole == EPlayerRole::PR_S_SERVICE ||
	//	PlayerRole == EPlayerRole::PR_A_ATTACK ||
	//	PlayerRole == EPlayerRole::PR_A_MOVE_TO_ATTACK_POS)
	//{
	//	show_ping_cursor_ = false;
	//	return;
	//}

	if (CanControlBallCursor)
	{
		show_ping_cursor_ = false;
		return;
	}

	auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!player_controller)
		return;

	FHitResult hit;
	player_controller->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hit);

	if (hit.bBlockingHit == false || hit.GetActor() == nullptr)
		return;

	bool is_on_ground = hit.GetActor()->ActorHasTag(FName("Land"));
	if (!is_on_ground)
		return;

	traced_in_team_court_ = UKismetMathLibrary::IsPointInBox(hit.Location, GetMyTeam()->team_box_->GetComponentLocation(), GetMyTeam()->team_box_->GetScaledBoxExtent());
	current_traced_pos_ = hit.Location;

	show_ping_cursor_ = true;
}

void ABasePlayer::TimingCalculateIfClick(float DeltaTime)
{
	if (bIsClicking && TimingMax > 0.0f)
	{
		// Timing To Accuracy
		TimingTimer += DeltaTime;
		TimingAccuracy = TimingTimer / TimingMax;
	}
	else
	{
		TimingAccuracy = 0.0f;
		TimingTimer = 0.0f;
		TimingMax = -1.0f;
	}
}