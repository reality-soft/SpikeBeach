// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
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

USTRUCT(BlueprintType)
struct FAnimationOffsetData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FAnimationOffsetData()
		: Left(0)
		, Right(0)
		, Front(0)
		, Back(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Left)
		FVector Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Right)
		FVector Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Front)
		FVector Front;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Back)
		FVector Back;
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

public:
	UPROPERTY(BlueprintReadWrite, Category = "Ball Attach Component")
		class USceneComponent* ball_attachment_;
protected:
	UPROPERTY(BlueprintReadOnly, Category = Animation)
		class UDataTable* AnimOffsetData;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool is_montage_started_ = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool is_montage_ended_ = false;

protected:
	UFUNCTION(BlueprintCallable, Category = Player)
		void MontageEnded();

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
	bool LoadDataTable();

protected:
	/** Called From AnimNotify for Service */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		void ServiceFloatingBall();
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void ServiceHitBall();

	// DEFENCE
	/** Called From AnimNotify for Receive */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void ReceiveBall();
	/** Called From AnimNotify for Dig */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void DigBall();
	/** Called From AnimNotify for Block */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void BlockBall();

	// OFFENCE
	/** Called From AnimNotify for Toss */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void TossBall();
	/** Called From AnimNotify for Pass */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void PassBall();
	/** Called From AnimNotify for Spike */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void SpikeBall();
	/** Called From AnimNotify for Floating */
	UFUNCTION(BlueprintCallable, Category = BallFunc)
		virtual void FloatingBall();

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
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetServiceMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetPassMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetAttackMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetReceiveMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetBlockMode();

protected:
	/* Play Service(Floating/Spoon/Jump) Animation */
	virtual void PlayServiceAnimation();

	/* Play Pass(Floating Pass / Toss) Animation */
	virtual void PlayPassAnimation();

	/* Play Attack(Spike/Floating Attack) Animation */
	virtual void PlayAttackAnimation();

	/* Play Receive(Dig/Receive) Animation */
	virtual void PlayReceiveAnimation();

	/* Play Block Animation */
	virtual void PlayBlockAnimation();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Animation)
	float GetMontageSectionLength(UAnimMontage* Montage, FName SectionName);
	/* To Play Animation in Accurate Timing, Calculate Play Rate */
	float CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName);
};
