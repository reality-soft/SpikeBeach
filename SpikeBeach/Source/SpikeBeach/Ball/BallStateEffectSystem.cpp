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

void UBallStateEffectSystem::ClearNiagaraComps()
{
	if (ngcomp_spline_tracks_.Num() > 0)
	{
		for (auto ng_spline : ngcomp_spline_tracks_)
		{
			if (ng_spline && ng_spline->IsActive())
			{
				ng_spline->DestroyComponent();
				ng_spline = nullptr;
			}
		}
		ngcomp_spline_tracks_.Empty();
	}

	if (ngcomp_landing_point_ && ngcomp_landing_point_->IsActive())
	{
		ngcomp_landing_point_->DestroyComponent();
		ngcomp_landing_point_ = nullptr;
	}
}

bool UBallStateEffectSystem::CreateSplineTrack()
{
	if (owner_ball_							 == nullptr ||
		owner_ball_->spline_comp_			 == nullptr || 
		owner_ball_->ngsystem_spline_track_  == nullptr ||
		owner_ball_->ngsystem_landing_point_ == nullptr)
		return false;

	if (ngcomp_landing_point_)
	{
		ngcomp_landing_point_->DestroyComponent();
		ngcomp_landing_point_ = nullptr;
	}
	for (UNiagaraComponent* ngcomp : ngcomp_spline_tracks_)
	{
		if (ngcomp)
		{
			ngcomp->DestroyComponent();
			ngcomp = nullptr;
		}
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> trace_types;
	TArray<AActor*, FDefaultAllocator> ignore_actors;
	trace_types.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ignore_actors.Add(owner_ball_);

	FHitResult hit_result;
	TArray<FVector> path_positions;
	FVector dest_position;

	bool success = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
		owner_ball_->GetWorld(), hit_result, path_positions, dest_position,
		owner_ball_->GetActorLocation(), owner_ball_->init_velocity_, true, owner_ball_->GetSphereComp()->GetScaledSphereRadius(),
		trace_types, true, ignore_actors, EDrawDebugTrace::None, 0.0f, 20.0f, 10.f);

	if (!success)
		return false;

	if (hit_result.GetActor())
	{
		if (hit_result.GetActor()->ActorHasTag("Land"))
		{
			owner_ball_->current_predict_.b_hit_land = true;
		}
		else
		{
			owner_ball_->current_predict_.b_hit_land = false;
		}
	}

	owner_ball_->current_predict_.destination = hit_result.Location;// dest_position;

	for (int32 index = 0; index < path_positions.Num(); ++index)
	{
		owner_ball_->spline_comp_->AddSplinePointAtIndex(path_positions[index], index, ESplineCoordinateSpace::World, true);
	}
	
	owner_ball_->spline_comp_->SetSplinePointType(path_positions.Max(),ESplinePointType::CurveClamped, true);
	owner_ball_->spline_positions_ = path_positions;

	for (int32 index = 0; index < path_positions.Num() - 2; ++index)
	{
		UNiagaraComponent* ngcomp_spline_track = UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner_ball_->GetWorld(), owner_ball_->ngsystem_spline_track_, path_positions[index], FRotator(0.f), FVector(1.0f), false);

		FVector step_dir = path_positions[index + 1] - path_positions[index];
		float pitch = FRotationMatrix::MakeFromX(step_dir).Rotator().Pitch / 360.0f * -1.0f;
		float yaw = FRotationMatrix::MakeFromZ(step_dir).Rotator().Yaw / 360.0f;
		ngcomp_spline_track->SetNiagaraVariableVec3("Vector", FVector(pitch, 0.0f, yaw));

		ngcomp_spline_tracks_.Add(ngcomp_spline_track);
	}

	ngcomp_landing_point_ = UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner_ball_->GetWorld(), owner_ball_->ngsystem_landing_point_, dest_position, FRotator(0.f), FVector(1.0f), false);

	return true;
}
