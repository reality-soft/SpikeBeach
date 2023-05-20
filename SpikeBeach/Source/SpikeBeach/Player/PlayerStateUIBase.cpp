// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUIBase.h"

bool UPlayerStateUIBase::InitReceiveRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim)
{
	stable_rg_img_ = img;
	stable_rg_size_ = size;
	stable_rg_anim_ = anim;

	return (stable_rg_img_ && stable_rg_size_ && stable_rg_anim_) ? true : false;
}

bool UPlayerStateUIBase::InitSpikeRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim)
{
	offensive_rg_img_ = img;
	offensive_rg_size_ = size;
	offensive_rg_anim_ = anim;
	
	return (offensive_rg_img_ && offensive_rg_size_ && offensive_rg_anim_) ? true : false;
}

void UPlayerStateUIBase::FillStableRG(float fill_value)
{
	rg_1_animtime_ += fill_value;
	stable_rg_img_->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(stable_rg_anim_, rg_1_animtime_);
}

void UPlayerStateUIBase::FillOffensiveRG(float fill_value)
{
	rg_2_animtime_ += fill_value;
	offensive_rg_img_->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(offensive_rg_anim_, rg_2_animtime_);
}

void UPlayerStateUIBase::LossStableRG()
{
	PauseAnimation(stable_rg_anim_);
}

void UPlayerStateUIBase::LossOffensiveRG()
{
	PauseAnimation(offensive_rg_anim_);
}

void UPlayerStateUIBase::UnshowStableRG()
{
	StopAnimation(stable_rg_anim_);
	rg_1_animtime_ = 0.0f;
	stable_rg_img_->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStateUIBase::UnshowOffensiveRG()
{
	StopAnimation(offensive_rg_anim_);
	rg_2_animtime_ = 0.0f;
	offensive_rg_img_->SetVisibility(ESlateVisibility::Hidden);
}
