// Fill out your copyright notice in the Description page of Project Settings.


#include "BallStateEffectSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
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

	return true;
}

void UBallStateEffectSystem::SpawnArcTrail()
{
	if (!owner_ball_ && !owner_ball_->ngsystem_arc_trail_)
		return;

	if (ngcomp_arc_trail_ != nullptr)
		DestroyArcTrail();

	ngcomp_arc_trail_ = UNiagaraFunctionLibrary::SpawnSystemAttached(owner_ball_->ngsystem_arc_trail_, owner_ball_->GetSphereComp(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false);
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("SubColor"), FLinearColor(1, 1, 1));
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("MainColor"), FLinearColor(1, 1, 1));
		ngcomp_arc_trail_->SetNiagaraVariableFloat(FString("SphereRad"), owner_ball_->GetSphereComp()->GetScaledSphereRadius());
	}
}

void UBallStateEffectSystem::SpawnSandDust()
{
	if (owner_ball_->ngsystem_sand_dust_ == nullptr)
		return;

	UNiagaraComponent* ngcomp_sand_dust = UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner_ball_->GetWorld(), owner_ball_->ngsystem_sand_dust_, owner_ball_->current_hit_floor_, FRotator(0.f), FVector(1.0f));
}

void UBallStateEffectSystem::DestroyArcTrail()
{
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->DestroyComponent();
		ngcomp_arc_trail_ = nullptr;
	}
}

void UBallStateEffectSystem::DestroyLandingPoint()
{
	if (ngcomp_landing_point_)
	{
		ngcomp_landing_point_->DestroyComponent();
		ngcomp_landing_point_ = nullptr;
	}
}

void UBallStateEffectSystem::SetArcTrailSpawnRate(float spawn_rate)
{
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->SetNiagaraVariableFloat(FString("SpawnRate"), spawn_rate);
	}
}

void UBallStateEffectSystem::SetTrailColor_Stable()
{
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("SubColor"), subcolor_stable_);
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("MainColor"), maincolor_stable_);
	}
}

void UBallStateEffectSystem::SetTrailColor_Offensive()
{
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("SubColor"), subcolor_offensive_);
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("MainColor"), maincolor_offensive_);
	}
}

void UBallStateEffectSystem::SetTrailColor_Wrong()
{
	if (ngcomp_arc_trail_)
	{
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("SubColor"), subcolor_wrong_);
		ngcomp_arc_trail_->SetNiagaraVariableLinearColor(FString("MainColor"), maincolor_wrong_);
	}
}