// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayBasePlayer.h"
#include "../Player/CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "../World/VolleyballTeam.h"

#include "Kismet/GameplayStatics.h"
#include "../SpikeBeachGameInstance.h"

#include "Packets/Requests/ControlReq.h"

// Sets default values
AMultiplayBasePlayer::AMultiplayBasePlayer() : ACharacter()
{
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void AMultiplayBasePlayer::BeginPlay()
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
void AMultiplayBasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto player_controller = Cast<ACustomPlayerController>(Controller);

	if (player_controller == nullptr)
		return;

	// ���� �ð��� ���ɴϴ�.
	float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

	// ť���� �̷� �ð��� ������ �Է��� ã���ϴ�.
	while (!movementInputQueue.empty())
	{
		FMovementInput CurMovementInput;
		CurMovementInput = movementInputQueue.front();

		// ó���� �Է��� ���� ���
		if (CurMovementInput.ApplyTime >= CurrentTime)
		{
			break;
		}
		else
		{
			movementInputQueue.pop_front();

			// ������ MovementVector�� ����� MovementVector�� �����մϴ�.
			curMovementVector = CurMovementInput.MovementVector;

			//// Vector2D�� X�� Y ���� OnScreenDebug �޽����� ����մϴ�.
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Applied");
			//FString DebugMessage = FString::Printf(TEXT("X: %f, Y: %f"), curMovementVector.X, curMovementVector.Y);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, DebugMessage);
		}
	}

	// Vector2D�� X�� Y ���� OnScreenDebug �޽����� ����մϴ�.
	//FString DebugMessage = FString::Printf(TEXT("X: %f, Y: %f"), curMovementVector.X, curMovementVector.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, DebugMessage);


	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, curMovementVector.Y);
	AddMovementInput(RightDirection, curMovementVector.X);

	//if (CanControlBallCursor)
	//{
	//	if (GetPlayerRole() == EPlayerRole::PR_S_SERVICE || GetPlayerRole() == EPlayerRole::PR_A_ATTACK)
	//	{
	//		dest_turnover_to_ = GetEnemyTeam()->ball_cursor_capsule_->GetComponentLocation();

	//		// Rotate To End Pos
	//		auto RotationDir = (dest_turnover_to_ - GetActorLocation());
	//		RotationDir.Z = 0;
	//		RotationDir.Normalize();
	//		auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), RotationDir);

	//		AddActorLocalRotation(quat);
	//	}
	//}

	//TimingCalculateIfClick(DeltaTime);

	//MouseTraceOnGround();

	// CheckClickableUI();

	//if (is_ping_clicked_)
	//{
	//	dest_position_ = current_ping_pos_;
	//}
	//else
	//{
	//	dest_position_ = GetActorLocation();
	//}
}

// Called to bind functionality to input
void AMultiplayBasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Clicking
		//EnhancedInputComponent->BindAction(LClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::LClickTriggered);
		//EnhancedInputComponent->BindAction(LClickAction, ETriggerEvent::Completed, this, &ABasePlayer::LClickCompleted);
		//EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::RClickTriggered);
		//EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Completed, this, &ABasePlayer::RClickCompleted);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiplayBasePlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMultiplayBasePlayer::Move);

		// Sprint
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABasePlayer::SprintTriggered);
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayer::SprintCompleted);

		//// Wheel Click
		//EnhancedInputComponent->BindAction(MouseWheelClickAction, ETriggerEvent::Triggered, this, &ABasePlayer::WheelTriggered);


		//// Ball Cursor
		//EnhancedInputComponent->BindAction(BallCursorControl, ETriggerEvent::Triggered, this, &ABasePlayer::BallCursorTriggered);
	}
}

void AMultiplayBasePlayer::Move(const FInputActionValue& Value)
{
	// If Moving to Action, Move Input disabled
	//if (is_montage_started_)
	//	return;

	FMovementInput movementToApply;

	// input is a Vector2D
	FVector2D curInputMovementVector = Value.Get<FVector2D>();

	// ���� �ֱ� �Է¿��� �Է��� ����� ��쿡�� ť�� ����
	if (movementInputQueue.empty() || curInputMovementVector != movementInputQueue.back().MovementVector)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Changed");

		USpikeBeachGameInstance* gameInstance = Cast<USpikeBeachGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		float DelayTime = gameInstance->getCurDelayTime() * 0.001f;
		float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

		FMovementInput MovementToApply;
		MovementToApply.MovementVector = curInputMovementVector;
		MovementToApply.ApplyTime = CurrentTime + DelayTime;
		movementInputQueue.push_back(MovementToApply);

		gameInstance->sendPacket<ControlReq>(curInputMovementVector);
	}
}