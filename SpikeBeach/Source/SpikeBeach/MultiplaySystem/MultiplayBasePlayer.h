// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <deque>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "InputActionValue.h"
#include "MultiplayBasePlayer.generated.h"





struct FMovementInput
{
	float ApplyTime;
	FVector2D MovementVector;
};

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API AMultiplayBasePlayer : public ACharacter
{
	GENERATED_BODY()

private:
#pragma region Input
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** LClick Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LClickAction;

	/** RClick Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RClickAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SprintAction;

	/** Mouse Wheel Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MouseWheelClickAction;

	/** Camera Length Control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CameraLengthControl;

	/** Ball Cursor Control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BallCursorControl;

#pragma endregion
	
public:
	// Sets default values for this character's properties
	AMultiplayBasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = Input)
		void Move(const FInputActionValue& Value);

	/** Queues for inputs */
	std::deque<FMovementInput> movementInputQueue;

	/** Current Movement Vector */
	FVector2D curMovementVector;
};
