// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerStateUISystem.h"
#include "BasePlayer.h"

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

}


bool UPlayerStateUISystem::InitInstances(ABasePlayer* player_, UCapsuleComponent* player_calsule, ACustomPlayerController* player_controller, UPlayerStateUIBase* state_ui)
{
	target_player_ = player_;
	player_capsule_ = player_calsule;
	player_controller_ = player_controller;
	player_state_ui_ = state_ui;

	return (target_player_ && player_capsule_ && player_controller_ && player_state_ui_) ? true : false;
}

bool UPlayerStateUISystem::InitRenderTransforms()
{
	return false;
}

FVector2D UPlayerStateUISystem::GetPlayerPosOnScreen()
{
	if (player_controller_ == nullptr)
		return FVector2D();

	FVector2D player_screen_pos;
	player_controller_->ProjectWorldLocationToScreen(target_player_->GetActorLocation(), player_screen_pos);
	return player_screen_pos;
}

// Called every frame
void UPlayerStateUISystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timer_manager_.Tick(DeltaTime);

	if (target_player_ == nullptr)
		return;

	if (player_pos_setted_ == false)
	{
		auto pos = GetPlayerPosOnScreen();
		if (!pos.IsNearlyZero())
		{
			player_pos_current_tick_ = pos;
			player_pos_setted_ = true;
		}
	}


	while (!target_player_->state_ui_notices_.IsEmpty())
	{
		EStateUINotice out_notice;
		target_player_->state_ui_notices_.Dequeue(out_notice);

		switch (out_notice)
		{
			// Update About Gauge
		case EStateUINotice::eActivateUI_StableRG:
			player_state_ui_->stable_rg_activation_ = true;
			player_state_ui_->SetStateUIVisible(EStateWidgets::eStableRG, true);
			break;
		case EStateUINotice::eActivateUI_OffensiveRG:
			player_state_ui_->offensive_rg_activation_ = true;
			player_state_ui_->SetStateUIVisible(EStateWidgets::eOffensiveRG, true);
			break;
		case EStateUINotice::eCloseUI_ReadyGauge:
			player_state_ui_->stable_rg_activation_ = false;
			player_state_ui_->offensive_rg_activation_ = false;
			player_state_ui_->SetStateUIVisible(EStateWidgets::eStableRG, false);
			player_state_ui_->SetStateUIVisible(EStateWidgets::eOffensiveRG, false);
			player_state_ui_->LossStableRG();
			player_state_ui_->LossOffensiveRG();
			break;
		}
	}

	player_state_ui_->FillStableRG(target_player_->GetTimingAccurancy());
	player_state_ui_->FillOffensiveRG(target_player_->GetTimingAccurancy());

	if (player_pos_setted_)
	{
		FVector2D dir = player_pos_last_tick_ - player_pos_current_tick_;
		player_pos_current_tick_ += dir * 0.05;

		player_state_ui_->stable_rg_parent_->SetRenderTranslation(player_pos_current_tick_ + ready_gauge_local_);
		player_state_ui_->offensive_rg_parent_->SetRenderTranslation(player_pos_current_tick_ + ready_gauge_local_);
		player_state_ui_->lclick_guide_parent_->SetRenderTranslation(player_pos_current_tick_ + lclick_guide_local_);
		player_state_ui_->rclick_guide_parent_->SetRenderTranslation(player_pos_current_tick_ + rclick_guide_local_);
	}

	if (target_player_->GetIsInBallTrigger())
	{
		player_state_ui_->SetStateUIVisible(EStateWidgets::eLClickGuide, true);
		player_state_ui_->SetStateUIVisible(EStateWidgets::eRClickGuide, true);
	}
	else
	{
		player_state_ui_->SetStateUIVisible(EStateWidgets::eLClickGuide, false);
		player_state_ui_->SetStateUIVisible(EStateWidgets::eRClickGuide, false);
	}
	if (player_state_ui_->lclick_guide_parent_->GetVisibility() == ESlateVisibility::Visible)
	{
		switch (target_player_->clickable_action_state_.LClick)
		{
		case EClickableAction::LClick_To_UnderService:
			player_state_ui_->ActiveLClickGuide(text_under_service_); break;
		case EClickableAction::LClick_To_StandingService:
			player_state_ui_->ActiveLClickGuide(text_standgind_service_); break;
		case EClickableAction::LClick_To_JumpService:
			player_state_ui_->ActiveLClickGuide(text_jump_service_); break;
		case EClickableAction::LClick_To_Receive:
			player_state_ui_->ActiveLClickGuide(text_receive_); break;
		case EClickableAction::LClick_To_Sliding:
			player_state_ui_->ActiveLClickGuide(text_sliding_); break;
		case EClickableAction::LClick_To_AttackFloat:
			player_state_ui_->ActiveLClickGuide(text_float_attack_); break;
		case EClickableAction::LClick_To_AttackSpike:
			player_state_ui_->ActiveLClickGuide(text_spike_attack_); break;
		}
	}

	if (player_state_ui_->rclick_guide_parent_->GetVisibility() == ESlateVisibility::Visible)
	{
		switch (target_player_->clickable_action_state_.RClick)
		{
		case EClickableAction::RClick_To_Pass:
			player_state_ui_->ActiveRClickGuide(text_pass_); break;
		case EClickableAction::RClick_To_Block:
			player_state_ui_->ActiveRClickGuide(text_block_); break;
		}
	}

	if (target_player_->show_ping_cursor_)
	{
		// ..Visible()
		player_state_ui_->ping_cursor_img_->SetVisibility(ESlateVisibility::Visible);

		if (target_player_->traced_in_team_court_)
		{
			// .. Set Green Color
			player_state_ui_->ping_cursor_img_->SetColorAndOpacity(FLinearColor(0, 1, 0, 1));
		}
		else
		{
			// .. Set Red Color
			player_state_ui_->ping_cursor_img_->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		}
	}
	else
	{
		// ..Hidden()
		player_state_ui_->ping_cursor_img_->SetVisibility(ESlateVisibility::Hidden);
		int32 viewport_x, viewport_y;

		player_controller_->GetViewportSize(viewport_x, viewport_y);
		player_controller_->SetMouseLocation(viewport_x / 2, viewport_y / 2);
	}
}
