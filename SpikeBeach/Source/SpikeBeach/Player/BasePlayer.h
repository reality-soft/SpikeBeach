// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseCharacter.h"
#include "InputActionValue.h"
#include "BasePlayer.generated.h"


enum class EStateUINotice
{
	eStartedGauge_StableType,
	eStartedGauge_OffensiveType,

	eFinishedGauge_StableType,
	eFinishedGauge_OffensiveType,

	eUnshowedGauge_StableType,
	eUnshowedGauge_OffensiveType,
};

UCLASS()
class SPIKEBEACH_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()


#pragma region Component

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

#pragma endregion

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
#pragma endregion


#pragma region UI
public:
	TQueue<EStateUINotice> state_ui_notices_;
#pragma endregion

#pragma region EFFECT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_timing_arm_ = nullptr;

#pragma endregion

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
	void SetCamera();
	void SetInputAction();

#pragma region GETTER
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
#pragma endregion

#pragma region SETTER
private:

#pragma endregion

protected:
	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = Input)
		void Move(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void LClickTriggered(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void LClickCompleted(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void RClickTriggered(const FInputActionValue& Value);
	/* Called for LClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void RClickCompleted(const FInputActionValue& Value);
	/* Called for Sprint Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void SprintTriggered(const FInputActionValue& Value);
	/* Called for Sprint Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void SprintCompleted(const FInputActionValue& Value);

protected:
	void MontageEnded() override;

protected:
	virtual void ServiceHitBall() override;

// DEFENCE
	virtual void ReceiveBall() override;
	virtual void DigBall() override;
	virtual void BlockBall() override;

// OFFENCE
	virtual void TossBall() override;
	virtual void PassBall() override;
	virtual void SpikeBall() override;
	virtual void FloatingBall() override;

protected:
	virtual void PlayServiceAnimation() override;
	virtual void PlayPassAnimation() override;
	virtual void PlayAttackAnimation() override;
	virtual void PlayReceiveAnimation() override;
	virtual void PlayBlockAnimation() override;
};
