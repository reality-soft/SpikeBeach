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


bool UPlayerStateUISystem::InitInstances(UCapsuleComponent* player_calsule, ACustomPlayerController* player_controller, UPlayerStateUIBase* state_ui)
{
	player_capsule_ = player_calsule;
	player_controller_ = player_controller;
	player_state_ui_ = state_ui;

	return (player_capsule_ && player_controller_ && player_state_ui_) ? true : false;
}

// Called every frame
void UPlayerStateUISystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set Receive RG Position
	if (player_state_ui_ && player_state_ui_->receive_rg_img_->GetVisibility() == ESlateVisibility::Visible)
	{
		FVector world_location = player_capsule_->GetComponentTransform().GetLocation();
		FVector2D screen_location;
		player_controller_->ProjectWorldLocationToScreen(world_location, screen_location);
		screen_location.X += -200.0f;
		screen_location.Y += -200.0f;

		player_state_ui_->receive_rg_img_->SetRenderTransform(FWidgetTransform(screen_location, FVector2D(1.0f, 1.0f), FVector2D(0.0f, 0.0f), 0.0f));
	}

	// Set Receive RG Position
	if (player_state_ui_ && player_state_ui_->spike_rg_img_->GetVisibility() == ESlateVisibility::Visible)
	{
		FVector world_location = player_capsule_->GetComponentTransform().GetLocation();
		FVector2D screen_location;
		player_controller_->ProjectWorldLocationToScreen(world_location, screen_location);
		screen_location.X += -200.0f;
		screen_location.Y += 200.0f;

		player_state_ui_->spike_rg_img_->SetRenderTransform(FWidgetTransform(screen_location, FVector2D(1.0f, 1.0f), FVector2D(0.0f, 0.0f), 0.0f));
	}
}
