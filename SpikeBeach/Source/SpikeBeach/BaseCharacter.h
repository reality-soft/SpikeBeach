// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerTurn : uint8
{
	PT_SERVICE,
	PT_DEFENCE,
	PT_OFFENCE,
	COUNT,
};

UENUM(BlueprintType)
enum class EOffenceMode : uint8
{
	OM_NONE,
	OM_TOSS,
	OM_PASS,
	OM_SPIKE,
	OM_FLOATING,
	COUNT,
};

UENUM(BlueprintType)
enum class EDefenceMode : uint8
{
	DM_NONE,
	DM_DIG,
	DM_RECEIVE,
	DM_BLOCK,
	COUNT,
};

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
class SPIKEBEACH_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Variables
	bool	bIsClicking;
	bool	bIsSprint;
	float	Gauge;
	float	TimingAccuracy;
	float	TimingTimer;
	float	TimingMax;

public:
	bool GetIsClicking() { return bIsClicking; }

	// Properties

#pragma region CHARACTER
protected:
	/* Guage Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float GaugeSpeed;

	/* Walk Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	/* Sprint Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
		float SprintSpeed;

	/* Player Turn */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		EPlayerTurn PlayerTurn;

	/* Attack Mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		EOffenceMode OffenceMode;

	/* Defence Mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		EDefenceMode DefenceMode;

	/* Service Mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		FName ServiceMode;

	/* Spike Mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		FName SpikeMode;

	/* Direction */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
		FName Direction;

#pragma endregion

#pragma region BALL INFO
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BallInfo, meta = (AllowPrivateAccess = "true"))
		float RemainingTimeToAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BallInfo, meta = (AllowPrivateAccess = "true"))
		FVector ActionPos;
#pragma endregion

#pragma region Animation
	/** Service Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ServiceMontage;

	// DEFENCE
	/** Receive Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ReceiveMontage;

	/** Dig Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DigMontage;

	/** Block Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BlockMontage;

	// OFFENCE
	/** Toss Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* TossMontage;

	/** Pass Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* PassMontage;

	/** Spike Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SpikeMontage;

	/** Floating Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* FloatingMontage;
#pragma endregion

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSuperSettings();
	void SetPlayerAttributes();
	void SetCapsuleComponent();
	void SetCharacterMovement();

protected:
	/** Called From AnimNotify for Service */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ServiceFloatingBall();
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ServiceHitBall();

	// DEFENCE
	/** Called From AnimNotify for Receive */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ReceiveBall();
	/** Called From AnimNotify for Dig */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void DigBall();
	/** Called From AnimNotify for Block */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void BlockBall();

	// OFFENCE
	/** Called From AnimNotify for Toss */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void TossBall();
	/** Called From AnimNotify for Pass */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void PassBall();
	/** Called From AnimNotify for Spike */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void SpikeBall();
	/** Called From AnimNotify for Floating */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void FloatingBall();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
		void JudgeServiceMode();
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
		void JudgePassMode();
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
		void JudgeAttackMode();
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
		void JudgeReceiveMode();
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
		void JudgeBlockMode();

protected:
	void	SetServiceMode();
	void	SetPassMode();
	void	SetAttackMode();
	void	SetReceiveMode();
	void	SetBlockMode();

protected:
	/* Play Service(Floating/Spoon/Jump) Animation */
	void PlayServiceAnimation();

	/* Play Pass(Floating Pass / Toss) Animation */
	void PlayPassAnimation();

	/* Play Attack(Spike/Floating Attack) Animation */
	void PlayAttackAnimation();

	/* Play Receive(Dig/Receive) Animation */
	void PlayReceiveAnimation();

	/* Play Block Animation */
	void PlayBlockAnimation();

protected:
	/* To Play Animation in Accurate Timing, Calculate Play Rate */
	float CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName);


};
