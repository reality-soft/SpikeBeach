// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"

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
	end_pos[2] = start_pos[2] + 1200;

	FVector direction_vector = end_pos - start_pos;
	direction_vector.Normalize();

	end_pos[2] = 50;

	SpikeHit(direction_vector, start_pos, end_pos);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::SpikeHit(FVector direction_vector, FVector start_pos, FVector end_pos)
{
	int used_index = (fabs(direction_vector[0]) > fabs(direction_vector[1])) ? 0 : 1;

	float total_time = sqrt(direction_vector[2] * (end_pos[used_index] - start_pos[used_index]) / (980.f * direction_vector[used_index]) + start_pos[2] / 980.f);

	float k = (end_pos[used_index] - start_pos[used_index]) / (total_time * direction_vector[used_index]);

	SphereCollisionComponent->AddImpulse(k * 11.5 * direction_vector);
}

