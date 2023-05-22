// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(SphereCollisionComponent);
	SphereCollisionComponent->SetSphereRadius(16.0f);
	SphereCollisionComponent->SetSimulatePhysics(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SphereCollisionComponent);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	FVector start_pos = GetActorLocation();
	FVector end_pos;
	end_pos[0] = start_pos[0] + 400;
	end_pos[1] = start_pos[1] - 1000;
	end_pos[2] = 400;

	ReceiveHit(1.0f, start_pos, end_pos);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	cur_time_ += DeltaTime;

	DropInfo drop_info = GetDropInfo(400);

	UE_LOG(LogTemp, Log, TEXT("Remain Time : %f"), drop_info.remain_time);
}

void ABall::SpikeHit(float power, const FVector& start_pos, const FVector& end_pos)
{
	power = (0.9 - 0.7) * power + 0.7;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	SphereCollisionComponent->SetAllPhysicsLinearVelocity(velocity, false);

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;
}

void ABall::ReceiveHit(float power, const FVector& start_pos, const FVector& end_pos)
{
	power = (0.7 - 0.3) * (1.0 - power) + 0.3;
	FVector velocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	SphereCollisionComponent->SetAllPhysicsLinearVelocity(velocity, false);

	// set drop data
	cur_time_ = 0.0f;
	start_pos_ = start_pos;
	end_pos_ = end_pos;
	init_velocity_ = velocity;
}

void ABall::PredictHitRoute(const FVector& velocity, const FVector& start_pos)
{
	FPredictProjectilePathParams params;
	params.bTraceWithChannel = true;
	params.bTraceWithCollision = true;
	params.LaunchVelocity = velocity;
	params.StartLocation = start_pos;

	FPredictProjectilePathResult result;
	UGameplayStatics::PredictProjectilePath(SphereCollisionComponent, params, result);
}

DropInfo ABall::GetDropInfo(float dest_height)
{
	DropInfo drop_info;
	
	float a_2 = 980.0f * 2;
	float minus_b = init_velocity_[2];
	float b_sqrd_minus_4ac_sqrt = sqrt(init_velocity_[2] * init_velocity_[2] - 4 * 980.0f * (dest_height - start_pos_[2]));

	float result1 = (minus_b + b_sqrd_minus_4ac_sqrt) / a_2;
	float result2 = (minus_b - b_sqrd_minus_4ac_sqrt) / a_2;

	float time_to_dest = (result1 > result2 ) ? result1 : result2;
	
	drop_info.drop_pos[0] = start_pos_[0] + time_to_dest * (init_velocity_[0]);
	drop_info.drop_pos[1] = start_pos_[1] + time_to_dest * (init_velocity_[1]);
	drop_info.drop_pos[2] = dest_height;

	drop_info.remain_time = time_to_dest - cur_time_;

	return drop_info;
}

