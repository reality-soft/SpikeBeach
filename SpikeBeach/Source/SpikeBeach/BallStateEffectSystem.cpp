// Fill out your copyright notice in the Description page of Project Settings.


#include "BallStateEffectSystem.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UBallStateEffectSystem::UBallStateEffectSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBallStateEffectSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBallStateEffectSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBallStateEffectSystem::InitOwnerInstance(ABall* ball)
{
	owner_ball_ = ball;
	if (!owner_ball_)
		return false;

	UNiagaraComponent* ng_comp = UNiagaraFunctionLibrary::SpawnSystemAttached(arc_trail_fx_, owner_ball_->GetSphereComp(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false);
	if (!ng_comp)
		return false;

	return true;
}

void UBallStateEffectSystem::ShowArcTrail()
{
	if (!owner_ball_)
		return;


	return;
}

