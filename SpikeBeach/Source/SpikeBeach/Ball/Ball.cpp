// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
			ProjectileMovementComponent->SetActive(false);
			break;

		case EBallState::eFloatToService:
			break;

		case EBallState::eStableSetted:
			break;

		case EBallState::eTurnOver:
			break;

		case EBallState::eMistake:
			break;

		case EBallState::eDropped:
			break;
		}
	}
}

FVector ABall::SpikeMovement(float power, const FVector& start_pos, const FVector& end_pos)
{
	power = (0.9 - 0.7) * power + 0.7;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	PushAndUpdateBallState(EBallState::eTurnOver);

	return velocity;
}

FVector ABall::ReceiveMovement(float power, const FVector& start_pos, const FVector& end_pos)
{
	power = (0.7 - 0.2) * (1.0 - power) + 0.2;
	FVector velocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = velocity;

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;

	PushAndUpdateBallState(EBallState::eStableSetted);

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

bool ABall::PushAndUpdateBallState(EBallState state)
{
	if (current_ball_state_ == state)
		return false;

	current_ball_state_ = state;
	state_queue_.Enqueue(state);

	return true;
}

