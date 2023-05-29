// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseCharacter.h"
#include "InputActionValue.h"
#include "BasePlayer.generated.h"


enum class EStateUINotice
{
	eActivateUI_StableRG,
	eActivateUI_OffensiveRG,

	eActivateUI_LClick_To_Service,
	eActivateUI_LClick_To_Receive,
	eActivateUI_LClick_To_Attack,

	eActivateUI_RClick_To_Pass,
	eActivateUI_RClick_To_Block,

	eCloseUI_ReadyGauge,
	eCloseUI_ClickGuide,
};

UCLASS()
class SPIKEBEACH_API ABasePlayer : public ABaseCharacter
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

	/** Camera Length Control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CameraLengthControl;

	/** Ball Cursor Control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BallCursorControl;

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
	void SetInputAction();

#pragma region GETTER
public:
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
	/* Called for MouseXY Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void BallCursorTriggered(const FInputActionValue& Value);

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


public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		bool CanControlBallCursor = false;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		FVector2D ball_cursor_value_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class AVolleyballArenaBase* arena_;

	UFUNCTION(BlueprintCallAble, Category = "Game Play")
		void ClearBallCursor();
};
