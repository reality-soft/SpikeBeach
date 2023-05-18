// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUIBase.h"

bool UPlayerStateUIBase::InitReceiveRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim)
{
	receive_rg_img_ = img;
	receive_rg_size_ = size;
	receive_rg_anim_ = anim;

	return (receive_rg_img_ && receive_rg_size_ && receive_rg_anim_) ? true : false;
}

bool UPlayerStateUIBase::InitSpikeRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim)
{
	spike_rg_img_ = img;
	spike_rg_size_ = size;
	spike_rg_anim_ = anim;
	
	return (spike_rg_img_ && spike_rg_size_ && spike_rg_anim_) ? true : false;
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
