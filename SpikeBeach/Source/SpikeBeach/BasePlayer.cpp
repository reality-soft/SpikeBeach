// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "CustomPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

bool ABasePlayer::GetIsGauging()
{
	return bIsGauging;
}

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
		FName Service = FName(TEXT("Spoon"));
		FName Direction = FName(TEXT("Left"));
		FName Spike = FName(TEXT("FullSpike"));
		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_SERVICE:
			PlayAnimMontage(ServiceMontage, 1.0f, Service);
			PlayerTurn = EPlayerTurn::PT_DEFENCE;
			break;
		case EPlayerTurn::PT_DEFENCE:
			PlayAnimMontage(ReceiveMontage, 1.0f, Direction);
			PlayerTurn = EPlayerTurn::PT_OFFENCE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			PlayAnimMontage(SpikeMontage, 1.0f, Direction);
			PlayerTurn = EPlayerTurn::PT_DEFENCE;
			break;
		}
	}

}

void ABasePlayer::LClickCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("LClick Complete"));

	bIsGauging = false;
}

void ABasePlayer::RClickTriggered(const FInputActionValue& Value)
{
	if (!bIsGauging)
	{
		bIsGauging = true;
		FName Direction = FName(TEXT("Front"));
		FName Spike = FName(TEXT("SemiSpike"));
		switch (PlayerTurn)
		{
		case EPlayerTurn::PT_DEFENCE:
			PlayAnimMontage(BlockMontage, 1.0f, Direction);
			PlayerTurn = EPlayerTurn::PT_OFFENCE;
			break;
		case EPlayerTurn::PT_OFFENCE:
			PlayAnimMontage(SpikeMontage, 1.0f, Spike);
			PlayerTurn = EPlayerTurn::PT_DEFENCE;
			break;
		}
	}

}

void ABasePlayer::RClickCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("RClick Complete"));

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
}

void ABasePlayer::ReceiveBall()
{
	UE_LOG(LogTemp, Log, TEXT("Receive Ball"));
}

void ABasePlayer::TossBall()
{
	UE_LOG(LogTemp, Log, TEXT("Toss Ball"));
}

void ABasePlayer::SpikeBall()
{
	UE_LOG(LogTemp, Log, TEXT("Spike Ball"));
}

void ABasePlayer::BlockBall()
{
	UE_LOG(LogTemp, Log, TEXT("Block Ball"));
}


