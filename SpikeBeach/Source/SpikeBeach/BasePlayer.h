// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasePlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerTurn : uint8
{
	PT_SERVICE,
	PT_DEFENCE,
	PT_OFFENCE,
	COUNT,
};

UCLASS()
class SPIKEBEACH_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

private:
	// Variables
	float Gauge;
	bool bIsGauging;

public:
	bool GetIsGauging();

	// Properties

	/* Guage Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float GaugeSpeed;

	/* Walk Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	/* Sprint Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float SprintSpeed;

	/* Player Click Turn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
		EPlayerTurn PlayerTurn;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

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

	/** Service Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ServiceMontage;

	/** Receive Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ReceiveMontage;

	/** Toss Toss Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* TossMontage;

	/** Spike Spike Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SpikeMontage;

	/** Block Spike Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BlockMontage;

public:
	// Sets default values for this character's properties
	ABasePlayer(); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SetSuperSettings();
	void SetPlayerAttributes();
	void SetCapsuleComponent();
	void SetCharacterMovement();
	void SetCamera();
	void SetInputAction();

protected:
	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = Input)
		void Move(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void LClickTriggered(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void LClickCompleted(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void RClickTriggered(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void RClickCompleted(const FInputActionValue& Value);
	/* Called for Sprint Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void SprintTriggered(const FInputActionValue& Value);
	/* Called for Sprint Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void SprintCompleted(const FInputActionValue& Value);

private:
	/** Called From AnimNotify for Service */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ServiceFloatingBall();
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ServiceHitBall();
	/** Called From AnimNotify for Receive */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ReceiveBall();
	/** Called From AnimNotify for Toss */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void TossBall();
	/** Called From AnimNotify for Spike */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void SpikeBall();
	/** Called From AnimNotify for Block */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void BlockBall();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
