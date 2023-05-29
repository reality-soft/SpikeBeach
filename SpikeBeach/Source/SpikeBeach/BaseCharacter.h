// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Delegates.h"
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

UENUM(BlueprintType)
enum class EFrontBack : uint8
{
	FRONT,
	BACK,
	COUNT,
};

UENUM(BlueprintType)
enum class ELeftRight : uint8
{
	LEFT,
	RIGHT,
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
		, Spoon(0)
		, Floating(0)
		, Jump(0)
		, FullSpike(0)
		, SemiSpike(0)
	{}
	// Direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Direction)
		FVector Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Direction)
		FVector Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Direction)
		FVector Front;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Direction)
		FVector Back;
	// Service
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Service)
		FVector Spoon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Service)
		FVector Floating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Service)
		FVector Jump;
	// Spike
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spike)
		FVector FullSpike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spike)
		FVector SemiSpike;
};


UCLASS()
class SPIKEBEACH_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	float GetTimmingAccurancy() { return TimingAccuracy; }

public:
	DECLARE_EVENT(ABaseCharacter, FAttackChange)
		FAttackChange TurnChangeEvent;

protected:
	// Variables
	bool	bIsClicking;
	bool	bIsSprint;
	float	TimingAccuracy;
	float	TimingTimer;
	float	TimingMax;

	UPROPERTY(BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		bool bIsInBallTrigger;

	UPROPERTY(BlueprintReadWrite, Category = Location, meta = (AllowPrivateAccess = "true"))
		EFrontBack front_back_;
	UPROPERTY(BlueprintReadWrite, Category = Location, meta = (AllowPrivateAccess = "true"))
		ELeftRight left_right_;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> ServiceOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> ReceiveOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> DigOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> BlockOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> SpikeOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> FloatingOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> TossOffsetMap;
	UPROPERTY(BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FVector> PassOffsetMap;
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
protected:
#pragma region GAME(Company, Ball) INFO
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameInfo, meta = (AllowPrivateAccess = "true"))
		class AActor* Company;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameInfo, meta = (AllowPrivateAccess = "true"))
		class ABall* Ball;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameInfo, meta = (AllowPrivateAccess = "true"))
		float RemainingTimeToAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameInfo, meta = (AllowPrivateAccess = "true"))
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
		void MontageStarted();
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void MontageEnded();

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetSuperSettings();
	void SetPlayerAttributes();
	void SetCapsuleComponent();
	void SetCharacterMovement();
	bool LoadDataTable();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TimingCalculateIfClick(float DeltaTime);
	void SmoothingWalkRun(float DeltaTime);
	

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
	//UFUNCTION(BlueprintImplementableEvent, Category = Player)
	//	void JudgePassMode();
	//UFUNCTION(BlueprintImplementableEvent, Category = Player)
	//	void JudgeAttackMode();
	//UFUNCTION(BlueprintImplementableEvent, Category = Player)
	//	void JudgeReceiveMode();
	//UFUNCTION(BlueprintImplementableEvent, Category = Player)
	//	void JudgeBlockMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		bool JudgePassMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		bool JudgeAttackMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		bool JudgeReceiveMode();
	UFUNCTION(BlueprintCallable, Category = Player)
		bool JudgeBlockMode();

	UFUNCTION(BlueprintCallable, Category = Player)
		FString GetPlayerMode();

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
	UFUNCTION(BlueprintCallable, Category = Animation)
		virtual void PlayServiceAnimation();

	/* Play Pass(Floating Pass / Toss) Animation */
	UFUNCTION(BlueprintCallable, Category = Animation)
		virtual void PlayPassAnimation();

	/* Play Attack(Spike/Floating Attack) Animation */
	UFUNCTION(BlueprintCallable, Category = Animation)
		virtual void PlayAttackAnimation();

	/* Play Receive(Dig/Receive) Animation */
	UFUNCTION(BlueprintCallable, Category = Animation)
		virtual void PlayReceiveAnimation();

	/* Play Block Animation */
	UFUNCTION(BlueprintCallable, Category = Animation)
		virtual void PlayBlockAnimation();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Animation)
		float GetMontageSectionLength(UAnimMontage* Montage, FName SectionName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Animation)
		float GetRequiredHeightFromOffset(const TMap<FName, FVector>& Map, FName name);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Character)
		FName GetDirectionFromPlayer(FVector TargetPos);

	/* To Play Animation in Accurate Timing, Calculate Play Rate */
	float CalculatePlayRate(float TimeRemaining, UAnimMontage* Montage, FName SectionName);

	/* Offset Rotate to Current Actor's Forward Vector */
	FVector RotateOffsetToCurrentDirection(FVector Vector);

	/* Move to Action Position : Set Speed to Action Pos through Remaining Time to Action */
	void MoveToActionPos(FVector Offset);

private:
	void HandleTurnChange();
};
