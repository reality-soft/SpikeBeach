// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class SPIKEBEACH_API ABall : public AActor
{
	GENERATED_BODY()
private:
	/** Sphere Collision Component */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollisionComponent;

	/** Mesh Component */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MeshComponent;
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#define MIN_POWER 500
#define MAX_POWER 2000

	void SpikeHit(float power, FVector start_pos, FVector end_pos);
	void ReceiveHit(float power, FVector start_pos, FVector end_pos);
	void PredictSpikePath(float power, FVector start_pos, FVector end_pos);
};
