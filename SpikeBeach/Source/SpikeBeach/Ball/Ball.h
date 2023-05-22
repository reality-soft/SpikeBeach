// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DropInfo.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		UNiagaraSystem* ngsystem_arc_trail_ = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectSystem")
		UNiagaraSystem* ngsystem_landing_point_ = nullptr;


private:
	float cur_time_ = 0.0f;
	FVector start_pos_;
	FVector end_pos_;
	FVector init_velocity_;

public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpikeHit(float power, const FVector& start_pos, const FVector& end_pos);
	void ReceiveHit(float power, const FVector& start_pos, const FVector& end_pos);
	void PredictHitRoute(const FVector& velocity, const FVector& start_pos);
	DropInfo GetDropInfo(float height);
	USphereComponent* GetSphereComp() { return SphereCollisionComponent; }

	void SpikeHit(FVector direction_vector, FVector start_pos, FVector end_pos);
};
