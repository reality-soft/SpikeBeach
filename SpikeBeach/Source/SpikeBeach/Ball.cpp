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
	end_pos[0] = start_pos[0] + 650;
	end_pos[1] = start_pos[1] - 1000;
	end_pos[2] = 0;

	SpikeHit(1.0f, start_pos, end_pos);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABall::SpikeHit(float power, FVector start_pos, FVector end_pos)
{
	power = (0.9 - 0.7) * power + 0.7;
	FVector velocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	SphereCollisionComponent->SetAllPhysicsLinearVelocity(velocity, false);
}

void ABall::ReceiveHit(float power, FVector start_pos, FVector end_pos)
{
	power = (0.7 - 0.3) * (1.0 - power) + 0.3;
	FVector velocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollisionComponent, velocity, start_pos, end_pos, 0.0f, power);

	SphereCollisionComponent->SetAllPhysicsLinearVelocity(velocity, false);
}

void ABall::PredictSpikePath(float power, FVector start_pos, FVector end_pos)
{
	FVector velocity = (end_pos - start_pos) * power;

	FPredictProjectilePathParams params;
	params.bTraceWithChannel = true;
	params.bTraceWithCollision = true;
	params.LaunchVelocity = velocity;
	params.StartLocation = start_pos;

	FPredictProjectilePathResult result;

	UGameplayStatics::PredictProjectilePath(SphereCollisionComponent, params, result);
}

