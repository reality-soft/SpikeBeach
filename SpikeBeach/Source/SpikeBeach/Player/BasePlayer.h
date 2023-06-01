// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseCharacter.h"
#include "InputActionValue.h"
#include "BasePlayer.generated.h"


enum class EStateUINotice
{
	eActivateUI_StableRG,
	eActivateUI_OffensiveRG,

	eCloseUI_ReadyGauge,
};

enum class EClickableAction
{
	Nothing,

	LClick_To_UnderService,
	LClick_To_StandingService,
	LClick_To_JumpService,
	LClick_To_Receive,
	LClick_To_Sliding,
	LClick_To_AttackFloat,
	LClick_To_AttackSpike,

	RClick_To_Pass,
	RClick_To_Block
};

UENUM(BlueprintType)
enum class EPingOrderType : uint8
{
	eWrongPos,
	ePassHere,
};

struct ClickableActionState
{
	EClickableAction LClick = EClickableAction::Nothing;
	EClickableAction RClick = EClickableAction::Nothing;
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


#pragma region UI
public:
	TQueue<EStateUINotice> state_ui_notices_;

	ClickableActionState clickable_action_state_;
#pragma endregion

#pragma region EFFECT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_timing_arm_ = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_pass_ping_ = nullptr;

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
	/* Called for MouseClick Input*/
	UFUNCTION(BlueprintCallable, Category = Input)
		void WheelTriggered(const FInputActionValue& Value);
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
	virtual bool JudgeServiceMode() override;
	virtual bool JudgePassMode() override;
	virtual bool JudgeAttackMode() override;
	virtual bool JudgeReceiveMode() override;
	virtual bool JudgeBlockMode() override;
protected:
	virtual void PlayServiceAnimation() override;
	virtual void PlayPassAnimation() override;
	virtual void PlayAttackAnimation() override;
	virtual void PlayReceiveAnimation() override;
	virtual void PlayBlockAnimation() override;
	void TimingCalculateIfClick(float DeltaTime);

public:
	FVector current_traced_pos_;
	bool traced_in_team_court_;
	bool show_ping_cursor_;
	void MouseTraceOnGround();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Effect System")
		void PingOrderEvent(EPingOrderType ping_type, FVector ping_location);

public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		bool CanControlBallCursor = false;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		FVector2D ball_cursor_value_;

};
