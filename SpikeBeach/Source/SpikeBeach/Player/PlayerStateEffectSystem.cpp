// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStateEffectSystem.h"
#include "BasePlayer.h"

// Sets default values for this component's properties
UPlayerStateEffectSystem::UPlayerStateEffectSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStateEffectSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UPlayerStateEffectSystem::InitOwnerInstance(ABasePlayer* player)
{
	if (player == nullptr)
		return false;

	target_player_ = player;
	return true;
}

void UPlayerStateEffectSystem::Spawn_TimingArm()
{
	if (!target_player_ && !target_player_->ngsystem_timing_arm_)
		return;

	if (ngcomp_timing_arm_ != nullptr)
		Destroy_TimingArm();

	ngcomp_timing_arm_ = UNiagaraFunctionLibrary::SpawnSystemAttached(target_player_->ngsystem_timing_arm_, target_player_->GetMesh(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false);
	if (ngcomp_timing_arm_)
	{
		ngcomp_timing_arm_->SetNiagaraVariableObject(FString("SKM"), target_player_->GetMesh());
		ngcomp_timing_arm_->SetNiagaraVariableLinearColor(FString("SubColor"), FLinearColor(1, 1, 1));
		ngcomp_timing_arm_->SetNiagaraVariableLinearColor(FString("MainColor"), FLinearColor(1, 1, 1));
		ngcomp_timing_arm_->SetNiagaraVariableFloat(FString("SpawnRate"), 30000);
	}
}

void UPlayerStateEffectSystem::Destroy_TimingArm()
{
	if (ngcomp_timing_arm_)
	{
		ngcomp_timing_arm_->DestroyComponent();
		ngcomp_timing_arm_ = nullptr;
	}
}

void UPlayerStateEffectSystem::SetSpawnRate_TimingArm(float spawn_rate)
{
	if (ngcomp_timing_arm_)
	{
		ngcomp_timing_arm_->SetNiagaraVariableFloat(FString("SpawnRate"), spawn_rate);
	}
}

void UPlayerStateEffectSystem::SetColorByTiming_TimingArm(float timing)
{
	if (ngcomp_timing_arm_)
	{
		FLinearColor result_subcolor = FMath::Lerp(subcolor_improper_, subcolor_proper_, timing);
		FLinearColor result_maincolor = FMath::Lerp(maincolor_improper_, maincolor_proper_, timing);
		ngcomp_timing_arm_->SetNiagaraVariableLinearColor(FString("SubColor"), result_subcolor);
		ngcomp_timing_arm_->SetNiagaraVariableLinearColor(FString("MainColor"), result_maincolor);
	}
}


// Called every frame
void UPlayerStateEffectSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

