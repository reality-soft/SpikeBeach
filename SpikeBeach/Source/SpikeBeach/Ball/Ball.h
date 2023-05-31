// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../Delegates.h"
#include "Ball.generated.h"

UENUM(BlueprintType)
enum class EBallState : uint8
{
	eNone,
	eAttached,
	eFloatToService,
	eStableSetted,
	eTurnOver,
	eMistake,
	eDropped,
};

USTRUCT(BlueprintType)
struct FDropInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FDropInfo()
		: drop_pos(0)
		, remain_time(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		FVector drop_pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float remain_time;
};

USTRUCT(BlueprintType)
struct FPredictInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Ball, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool b_hit_land; 

	UPROPERTY(Category = Ball, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector destination;

	UPROPERTY(BlueprintReadOnly, Category = "Spline Track")
		TArray<FVector> spline_positions_;
};

UCLASS()
class SPIKEBEACH_API ABall : public AActor
{
	GENERATED_BODY()
private:
	/** Sphere Collision Component */
	UPROPERTY(Category = Ball, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollisionComponent;

	/** Mesh Component */
	UPROPERTY(Category = Ball, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovementComponent;

	/** Mesh Component */
	UPROPERTY(Category = Ball, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MeshComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_arc_trail_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_landing_point_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		class UNiagaraSystem* ngsystem_sand_dust_;

	// Spline Track
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Track")
		class USplineComponent* spline_comp_;

	UPROPERTY(BlueprintReadWrite, Category = "Sand Dust")
		FVector current_hit_floor_;

	// Ball State
	UPROPERTY(BlueprintReadOnly, Category = "Ball State")
		EBallState current_ball_state_ = EBallState::eNone;

	UPROPERTY(BlueprintReadOnly, Category = "Ball State")
		FPredictInfo current_predict_;

	TQueue<EBallState> state_queue_;

	UPROPERTY(BlueprintReadWrite, Category = "EffectSystem")
		class UBallStateEffectSystem* parent_effect_system_;


public:
	UPROPERTY(BlueprintReadOnly, Category = DropInfo)
		float cur_time_ = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = DropInfo)
		FVector start_pos_;
	UPROPERTY(BlueprintReadOnly, Category = DropInfo)
		FVector end_pos_;
	UPROPERTY(BlueprintReadOnly, Category = DropInfo)
		FVector init_velocity_;

public:
	DECLARE_EVENT(ABall, FAttackChange)
		FAttackChange TurnChangeDelegate;

public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateByBallState();
	void CheckTurnChanged();

	UFUNCTION(BlueprintCallable, Category = "Ball State")
		bool PushAndUpdateBallState(EBallState state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector SpikeMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);
	
	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector ReceiveMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector TossMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector DigMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector FloatingMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector SpoonServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector JumpServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		FVector FloatingServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state);

	UFUNCTION(BlueprintCallable, Category = "Ball Movement")
		void NetHitMovement(const FVector& hit_location, const FVector& impulse_normal);

	UFUNCTION(BlueprintCallable, Category = "Ball System")
		void PredictPath();

	UFUNCTION(BlueprintCallable, Category = BallFunc)
		FDropInfo GetDropInfo(float height);

	USphereComponent* GetSphereComp() { return SphereCollisionComponent; }
	UProjectileMovementComponent* GetProjectileComp() { return ProjectileMovementComponent; }
};
