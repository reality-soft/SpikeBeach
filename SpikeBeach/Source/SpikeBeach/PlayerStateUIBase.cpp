// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUIBase.h"

bool UPlayerStateUIBase::InitRGImages(UImage* receive_rg_img, UImage* spike_rg_img, UWidgetAnimation* receive_rg_anim, UWidgetAnimation* spike_rg_anim)
{
	receive_rg_img_ = receive_rg_img;
	spike_rg_img_ = spike_rg_img;
	receive_rg_anim_ = receive_rg_anim;
	spike_rg_anim_ = spike_rg_anim;

	return (receive_rg_img_ && spike_rg_img_ && receive_rg_anim_ && spike_rg_anim_) ? true : false;
}

void UPlayerStateUIBase::FillReceiveRG(float fill_value)
{
	rg_1_animtime_ += fill_value;
	receive_rg_img_->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(receive_rg_anim_, rg_1_animtime_);
}

void UPlayerStateUIBase::FillSpikeRG(float fill_value)
{
	rg_2_animtime_ += fill_value;
	spike_rg_img_->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(spike_rg_anim_, rg_2_animtime_);
}

void UPlayerStateUIBase::LossReceiveRG()
{
	rg_1_animtime_ = 0.0f;
	receive_rg_img_->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(receive_rg_anim_);
}


void UPlayerStateUIBase::LossSpikeRG()
{
	rg_2_animtime_ = 0.0f;
	spike_rg_img_->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(spike_rg_anim_);
}
