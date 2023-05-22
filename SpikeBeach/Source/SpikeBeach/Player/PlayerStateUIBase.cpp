// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUIBase.h"
#include "Materials/MaterialInstanceDynamic.h"

bool UPlayerStateUIBase::InitReceiveRGInstances(UImage* img, USizeBox* size, UMaterialInstanceDynamic* mi)
{
	stable_rg_img_ = img;
	stable_rg_size_ = size;
	stable_rg_mi_ = mi;

	return (stable_rg_img_ && stable_rg_size_ && stable_rg_mi_) ? true : false;
}

bool UPlayerStateUIBase::InitSpikeRGInstances(UImage* img, USizeBox* size, UMaterialInstanceDynamic* mi)
{
	offensive_rg_img_ = img;
	offensive_rg_size_ = size;
	offensive_rg_mi_ = mi;

	return (offensive_rg_img_ && offensive_rg_size_ && offensive_rg_mi_) ? true : false;
}

void UPlayerStateUIBase::FillStableRG(float fill_value)
{
	stable_rg_mi_->SetScalarParameterValue("AnimPhase", fill_value);
	stable_rg_img_->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStateUIBase::FillOffensiveRG(float fill_value)
{
	offensive_rg_mi_->SetScalarParameterValue("AnimPhase", fill_value);
	stable_rg_img_->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStateUIBase::LossStableRG()
{
}

void UPlayerStateUIBase::LossOffensiveRG()
{
}

void UPlayerStateUIBase::UnshowStableRG()
{
	stable_rg_mi_->SetScalarParameterValue("AnimPhase", 0.0f);
	stable_rg_img_->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStateUIBase::UnshowOffensiveRG()
{
	offensive_rg_mi_->SetScalarParameterValue("AnimPhase", 0.0f);
	offensive_rg_img_->SetVisibility(ESlateVisibility::Hidden);
}
