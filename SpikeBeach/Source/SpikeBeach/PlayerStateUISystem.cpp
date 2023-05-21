// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUISystem.h"

// Sets default values for this component's properties
UPlayerStateUISystem::UPlayerStateUISystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStateUISystem::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


bool UPlayerStateUISystem::InitInstances(ABasePlayer* player_, UCapsuleComponent* player_calsule, ACustomPlayerController* player_controller, UPlayerStateUIBase* state_ui)
{
	target_player_ = player_;
	player_capsule_ = player_calsule;
	player_controller_ = player_controller;
	player_state_ui_ = state_ui;

	return (target_player_ && player_capsule_ && player_controller_ && player_state_ui_) ? true : false;
}

// Called every frame
void UPlayerStateUISystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (target_player_->GetIsClicking())
	{
		player_state_ui_->FillSpikeRG(DeltaTime * spike_proficiency);
	}
	else
	{
		player_state_ui_->LossSpikeRG();
	}

	// Set Receive RG Position
	if (player_state_ui_ && player_state_ui_->receive_rg_img_->GetVisibility() == ESlateVisibility::Visible)
	{
		player_state_ui_->receive_rg_img_->SetRenderTranslation(player_screen_pos_ + receive_rg_local_);
	}

	// Set Spike RG Position
	if (player_state_ui_ && player_state_ui_->spike_rg_img_->GetVisibility() == ESlateVisibility::Visible)
	{
		player_state_ui_->spike_rg_img_->SetRenderTranslation(player_screen_pos_ + spike_rg_local_);
	}
}
