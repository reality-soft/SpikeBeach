// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "../World/VolleyballArenaBase.h"
#include "../World/VolleyballTeam.h"
#include "../World/VolleyballGame.h"
#include "../Ball/Ball.h"
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

	auto player_controller = Cast<ACustomPlayerController>(Controller);

	if (player_controller == nullptr)
		return;

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

	CheckClickableUI();

	if (is_ping_clicked_)
	{
		dest_position_ = current_traced_pos_;
	}
	else
	{
		dest_position_ = GetActorLocation();
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
		PingOrderEvent(EPingOrderType::ePassHere, current_traced_pos_);
		is_ping_clicked_ = true;
	}
	else
	{
		PingOrderEvent(EPingOrderType::eWrongPos, current_traced_pos_);
		is_ping_clicked_ = false;
	}	
}

void ABasePlayer::BallCursorTriggered(const FInputActionValue& Value)
{
	if (my_team_ == nullptr)
		return;

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

	TimingAccuracy = 0.0f;

	state_ui_notices_.Enqueue(EStateUINotice::eCloseUI_ReadyGauge);
}


void ABasePlayer::ServiceHitBall()
{
	UE_LOG(LogTemp, Log, TEXT("Service : Hit Ball"));

	if (Ball)
	{
		FVector StartPos = Ball->GetActorLocation();
		FVector EndPos = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();
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
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->SpikeMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());

	CanControlBallCursor = false;
}

void ABasePlayer::FloatingBall()
{
	bIsMoveToOffset = false;
	OffsetTimer = 0;
	UE_LOG(LogTemp, Log, TEXT("Floating Ball"));

	FVector StartPos = Ball->GetActorLocation();
	FVector EndPos = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();
	EndPos = GetRandomPosInRange(EndPos, TimingAccuracy);

	Ball->FloatingMovement(1.2, StartPos, EndPos, EBallState::eTurnOver);
	Ball->SetLastTouchCourt(GetMyTeam()->GetCourtName());

	CanControlBallCursor = false;
}

bool ABasePlayer::JudgeServiceMode()
{
	if (is_montage_started_)
		return false;
	return ABaseCharacter::JudgeServiceMode();
}

EOffenceMode ABasePlayer::JudgePassMode()
{
	if (is_montage_started_)
		return EOffenceMode::OM_NONE;
	return ABaseCharacter::JudgePassMode();
}

EOffenceMode ABasePlayer::JudgeAttackMode()
{
	if (is_montage_started_)
		return EOffenceMode::OM_NONE;
	return ABaseCharacter::JudgeAttackMode();
}

EDefenceMode ABasePlayer::JudgeReceiveMode()
{
	if (is_montage_started_)
		return EDefenceMode::DM_NONE;
	return ABaseCharacter::JudgeReceiveMode();
}

EDefenceMode ABasePlayer::JudgeBlockMode()
{
	if (is_montage_started_)
		return EDefenceMode::DM_NONE;
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
	if (my_team_ == nullptr)
		return;

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
		TimingTimer = 0.0f;
		TimingMax = -1.0f;
	}
}

void ABasePlayer::CheckClickableUI()
{
	switch (PlayerTurn)
	{
	case EPlayerTurn::PT_SERVICE:
	{
		if (ServiceMode == FName("Jump"))
			clickable_action_state_.LClick = EClickableAction::LClick_To_JumpService;
		if (ServiceMode == FName("Floating"))
			clickable_action_state_.LClick = EClickableAction::LClick_To_StandingService;
		if (ServiceMode == FName("Spoon"))
			clickable_action_state_.LClick = EClickableAction::LClick_To_UnderService;

		clickable_action_state_.RClick = EClickableAction::Nothing;
		break;
	}
	case EPlayerTurn::PT_DEFENCE:
	{
		if (is_montage_started_)
		{
			clickable_action_state_.LClick = EClickableAction::Nothing;
			clickable_action_state_.RClick = EClickableAction::Nothing;
			return;
		}

		// L
		switch (GetReceiveUIMode())
		{
		case EDefenceMode::DM_RECEIVE:
			clickable_action_state_.LClick = EClickableAction::LClick_To_Receive;
			break;
		case EDefenceMode::DM_DIG:
			clickable_action_state_.LClick = EClickableAction::LClick_To_Sliding;
			break;
		case EDefenceMode::DM_NONE:
			clickable_action_state_.LClick = EClickableAction::Nothing;
			break;
		}

		// R
		clickable_action_state_.RClick = EClickableAction::RClick_To_Block;

		break;
	}
	case EPlayerTurn::PT_OFFENCE:
	{

		if (is_montage_started_)
		{
			clickable_action_state_.LClick = EClickableAction::Nothing;
			clickable_action_state_.RClick = EClickableAction::Nothing;
			return;
		}

		// L
		switch (GetAttackUIMode())
		{
		case EOffenceMode::OM_SPIKE:
			clickable_action_state_.LClick = EClickableAction::LClick_To_AttackSpike;
			break;
		case EOffenceMode::OM_FLOATING:
			clickable_action_state_.LClick = EClickableAction::LClick_To_AttackFloat;
			break;
		case EOffenceMode::OM_NONE:
			clickable_action_state_.LClick = EClickableAction::Nothing;
			break;
		}
		// R
		switch (GetPassUIMode())
		{
		case EOffenceMode::OM_TOSS:
			clickable_action_state_.RClick = EClickableAction::RClick_To_Pass;
			break;
		case EOffenceMode::OM_PASS:
			clickable_action_state_.RClick = EClickableAction::RClick_To_Pass;
			break;
		case EOffenceMode::OM_NONE:
			clickable_action_state_.RClick = EClickableAction::Nothing;
			break;
		}

		break;
	}
	}
}

EOffenceMode ABasePlayer::GetPassUIMode()
{
	if (!Company)
		return EOffenceMode::OM_NONE;

	if (GetEnemyTeam()->IsVectorInTeamBox(Ball->end_pos_))
		return EOffenceMode::OM_NONE;

	float TimeToPlayAnim = 0;
	float RequiredHeight = 0;
	FDropInfo DropInfo;

	// 1. Set Direction
	auto direction = GetDirectionFromPlayer(Company->GetActorLocation());

	if (trigger_state_ & (uint8)EActionTriggerState::TS_TOSS)
	{
		// 2. Check If Toss is Possible
		FName FilterName = direction.Compare(FName("Back")) == 0 ? FName("Front") : direction;
		RequiredHeight = GetRequiredHeightFromOffset(TossOffsetMap, FilterName);
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = TossMontage->GetSectionLength(TossMontage->GetSectionIndex(FilterName));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
		{
			return EOffenceMode::OM_TOSS;
		}
	}

	if (trigger_state_ & (uint8)EActionTriggerState::TS_PASS)
	{
		// 4. Check If Pass is Possible
		RequiredHeight = GetRequiredHeightFromOffset(PassOffsetMap, direction);
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 5. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = PassMontage->GetSectionLength(PassMontage->GetSectionIndex(direction));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
		{
			return EOffenceMode::OM_PASS;
		}
	}

	return EOffenceMode::OM_NONE;
}

EOffenceMode ABasePlayer::GetAttackUIMode()
{
	if (!Company)
		return EOffenceMode::OM_NONE;

	if (GetEnemyTeam()->IsVectorInTeamBox(Ball->end_pos_))
		return EOffenceMode::OM_NONE;

	float TimeToPlayAnim = 0;
	float RequiredHeight = 0;
	FDropInfo DropInfo;

	if (trigger_state_ & (uint8)EActionTriggerState::TS_SPIKE)
	{
		// 1. Set Spike
		SpikeMode = FName("FullSpike");

		// 2. Check If Spike is Possible
		RequiredHeight = GetRequiredHeightFromOffset(SpikeOffsetMap, SpikeMode);
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = SpikeMontage->GetSectionLength(SpikeMontage->GetSectionIndex(SpikeMode));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
		{
			return EOffenceMode::OM_SPIKE;
		}
	}

	if (trigger_state_ & (uint8)EActionTriggerState::TS_FLOATING)
	{
		// 4. Set Direction to Enemy's Court : TODO
		auto direction = FName("Front");

		// 5. Check If Floating is Possible
		RequiredHeight = GetRequiredHeightFromOffset(FloatingOffsetMap, direction);
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 6. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = FloatingMontage->GetSectionLength(FloatingMontage->GetSectionIndex(direction));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
		{
			return EOffenceMode::OM_FLOATING;
		}
	}

	return EOffenceMode::OM_NONE;
}

EDefenceMode ABasePlayer::GetReceiveUIMode()
{
	if (!Company)
		return EDefenceMode::DM_NONE;

	if (!GetMyTeam()->IsVectorInTeamBox(Ball->end_pos_))
		return EDefenceMode::DM_NONE;

	float TimeToPlayAnim = 0;
	float RequiredHeight = 0;
	FDropInfo DropInfo;

	// 1. Set Direction
	auto direction = GetDirectionFromPlayer(Company->GetActorLocation());

	if (trigger_state_ & (uint8)EActionTriggerState::TS_RECEIVE)
	{
		// 2. Check If Receive is Possible
		RequiredHeight = GetRequiredHeightFromOffset(ReceiveOffsetMap, direction);
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 3. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = ReceiveMontage->GetSectionLength(ReceiveMontage->GetSectionIndex(direction));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.7f)
		{
			return EDefenceMode::DM_RECEIVE;
		}
	}

	if (trigger_state_ & (uint8)EActionTriggerState::TS_DIG)
	{
		// 4. Check If Dig is Possible
		RequiredHeight = GetRequiredHeightFromOffset(DigOffsetMap, FName("Front"));
		DropInfo = Ball->GetDropInfo(RequiredHeight);

		// 5. If Remaining Time to Arrive is more than AnimTime, Waiting
		TimeToPlayAnim = DigMontage->GetSectionLength(DigMontage->GetSectionIndex(direction));

		if (DropInfo.remain_time > TimeToPlayAnim * 0.3f)
		{
			return EDefenceMode::DM_DIG;
		}
	}

	return EDefenceMode::DM_NONE;
}
