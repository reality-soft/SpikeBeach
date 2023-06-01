// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BallStateEffectSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(SphereCollisionComponent);
	SphereCollisionComponent->SetSphereRadius(16.0f);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SphereCollisionComponent);

	spline_comp_ = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	//ReceiveMovement(0.3f, GetActorLocation(), FVector(2000, 810, 25));
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateByBallState();

	cur_time_ += DeltaTime;
}

void ABall::UpdateByBallState()
{
	while (state_queue_.IsEmpty() == false)
	{
		EBallState state = EBallState::eNone;
		state_queue_.Dequeue(state);

		switch (state)
		{
		case EBallState::eNone:
			break;
		case EBallState::eAttached:
			parent_effect_system_->EventClearSplineTrack();
			parent_effect_system_->SetArcTrailSpawnRate(0);
			ProjectileMovementComponent->SetActive(false);
			break;

		case EBallState::eFloatToService:
			parent_effect_system_->EventClearSplineTrack();
			parent_effect_system_->SetArcTrailSpawnRate(1500);
			parent_effect_system_->SetTrailColor_Stable();
			break;

		case EBallState::eStableSetted:
			PredictPath();
			parent_effect_system_->EventCreateSplineTrack(spline_comp_, current_predict_.spline_positions_);
			parent_effect_system_->SetTrailColor_Stable();
			parent_effect_system_->SetArcTrailSpawnRate(3000);
			break;

		case EBallState::eTurnOver:
			PredictPath();
			parent_effect_system_->EventCreateSplineTrack(spline_comp_, current_predict_.spline_positions_);
			parent_effect_system_->SetTrailColor_Offensive();
			parent_effect_system_->SetArcTrailSpawnRate(3000);
			break;

		case EBallState::eMistake:
			PredictPath();
			parent_effect_system_->EventCreateSplineTrack(spline_comp_, current_predict_.spline_positions_);
			parent_effect_system_->SetTrailColor_Wrong();
			parent_effect_system_->SetArcTrailSpawnRate(3000);
			break;

		case EBallState::eDropped:
			parent_effect_system_->EventClearSplineTrack();
			parent_effect_system_->SetArcTrailSpawnRate(0);
			parent_effect_system_->SpawnSandDust();
			break;
		}
	}
}

FVector ABall::SpikeMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.5) * power + 0.5;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;
	
	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::ReceiveMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.3) * (1.0 - power) + 0.3;
	FVector velocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::TossMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.3) * (1.0 - power) + 0.3;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::FloatingMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.4) * (1.0 - power) + 0.4;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::DigMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.5) * (1.0 - power) + 0.5;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::JumpServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.7 - 0.6) * power + 0.6;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FVector ABall::FloatingServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.5 - 0.4) * power + 0.4;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

void ABall::SetLastTouchCourt(ECourtName court)
{
	LastTouchCourt = court;
}

void ABall::NetHitMovement(const FVector& hit_location, const FVector& impulse_normal)
{	
	double reduced_speed = std::max(GetVelocity().Length() * 0.25, 50.0);
	FVector movement_velocity = impulse_normal * reduced_speed;

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = movement_velocity;

	init_velocity_ = movement_velocity;
	PredictPath();

	if (current_predict_.b_hit_land)
	{
		parent_effect_system_->EventCreateSplineTrack(spline_comp_, current_predict_.spline_positions_);
		parent_effect_system_->SetTrailColor_Stable();
		parent_effect_system_->SetArcTrailSpawnRate(3000);
	}

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = hit_location;
	end_pos_ = current_predict_.destination;

	is_valid_attack_ = true;

	CheckTurnChanged();
}

void ABall::BlockHitMovement(const FVector& hit_location, const FVector& impulse_normal)
{
	FVector movement_velocity = impulse_normal * GetVelocity().Length() * 0.6f;

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = movement_velocity;

	init_velocity_ = movement_velocity;
	PredictPath();

	if (current_predict_.b_hit_land)
	{
		parent_effect_system_->EventCreateSplineTrack(spline_comp_, current_predict_.spline_positions_);
		parent_effect_system_->SetTrailColor_Stable();
		parent_effect_system_->SetArcTrailSpawnRate(3000);
	}

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = hit_location;
	end_pos_ = current_predict_.destination;

	PushAndUpdateBallState(EBallState::eTurnOver);

	is_valid_attack_ = true;

	CheckTurnChanged();
}

FVector ABall::SpoonServiceMovement(float power, const FVector& start_pos, const FVector& end_pos, EBallState ball_state)
{
	power = (0.4 - 0.3) * power + 0.3;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	CheckTurnChanged();
	PushAndUpdateBallState(ball_state);

	is_valid_attack_ = true;

	return velocity;
}

FDropInfo ABall::GetDropInfo(float dest_height)
{
	float gravity = -980.0f;
	FDropInfo drop_info;

	float a_2 = gravity;
	float minus_b = -init_velocity_[2];
	float b_sqrd_minus_4ac_sqrt = sqrt(init_velocity_[2] * init_velocity_[2] -2 * gravity * (start_pos_[2] - dest_height));

	float result1 = (minus_b + b_sqrd_minus_4ac_sqrt) / a_2;
	float result2 = (minus_b - b_sqrd_minus_4ac_sqrt) / a_2;

	float time_to_dest = (result1 > result2) ? result1 : result2;

	drop_info.drop_pos[0] = start_pos_[0] + time_to_dest * (init_velocity_[0]);
	drop_info.drop_pos[1] = start_pos_[1] + time_to_dest * (init_velocity_[1]);
	drop_info.drop_pos[2] = dest_height;

	drop_info.remain_time = time_to_dest - cur_time_;

	return drop_info;

}

void ABall::PredictPath()
{
	if (spline_comp_ == nullptr ||
		ngsystem_landing_point_ == nullptr)
		return;

	current_predict_.spline_positions_.Empty();
	spline_comp_->ClearSplinePoints();

	TArray<TEnumAsByte<EObjectTypeQuery>> trace_types;
	TArray<AActor*, FDefaultAllocator> ignore_actors;
	trace_types.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ignore_actors.Add(this);

	FHitResult hit_result;
	TArray<FVector> path_positions;
	FVector dest_position;

	bool success = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
		GetWorld(), hit_result, path_positions, dest_position,
		GetActorLocation(), init_velocity_, true, GetSphereComp()->GetScaledSphereRadius(),
		trace_types, false, ignore_actors, EDrawDebugTrace::None, 0.0f, 5.0f, 10.f);

	if (success && hit_result.bBlockingHit)
	{
		if (hit_result.GetActor()->ActorHasTag("Land"))
		{
			current_predict_.b_hit_land = true;
		}
		else
		{
			current_predict_.b_hit_land = false;
		}

		current_predict_.destination = hit_result.Location;
		current_predict_.spline_positions_ = path_positions;

		for (int32 index = 0; index < path_positions.Num(); ++index)
		{
			spline_comp_->AddSplinePointAtIndex(path_positions[index], index, ESplineCoordinateSpace::World, true);
		}
	}


	return;
}

bool ABall::PushAndUpdateBallState(EBallState state)
{
	//if (current_ball_state_ == state)
	//	return false;
	current_ball_state_ = state;
	state_queue_.Enqueue(state);

	return true;
}

void ABall::CheckTurnChanged() 
{
	// never reach net
	if ((start_pos_[1] > 810.0f && init_velocity_[1] > 0.0f) || (start_pos_[1] < 810.0f && init_velocity_[1] < 0.0f)) {
		return;
	}

	float time_to_dest = (810.0f - start_pos_[1]) / init_velocity_[1];

	// height check
	float gravity = -980.0f;
	float height = start_pos_[2] + init_velocity_[2] * time_to_dest + 0.5f * gravity * time_to_dest * time_to_dest;
	if (height < 205.0f) {
		return;
	}

	// horizontal_coord_check
	float horizontal_coord = start_pos_[0] + init_velocity_[0] * time_to_dest;
	if (horizontal_coord < 1610.0f || 2370.0f < horizontal_coord) {
		return;
	}

	TurnChangeDelegate.Broadcast();
}