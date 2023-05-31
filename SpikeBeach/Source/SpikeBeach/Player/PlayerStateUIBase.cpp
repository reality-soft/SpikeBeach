// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUIBase.h"
#include "Materials/MaterialInstanceDynamic.h"

bool UPlayerStateUIBase::InitReceiveRGInstances(UWidget* parent_widget, UImage* img, UMaterialInstanceDynamic* mi)
{
	stable_rg_parent_ = parent_widget;
	stable_rg_img_ = img;
	stable_rg_mi_ = mi;

	if (stable_rg_parent_ && stable_rg_img_ && stable_rg_mi_)
	{
		stable_rg_parent_->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}

	return false;
}

bool UPlayerStateUIBase::InitSpikeRGInstances(UWidget* parent_widget, UImage* img, UMaterialInstanceDynamic* mi)
{
	offensive_rg_parent_ = parent_widget;
	offensive_rg_img_ = img;
	offensive_rg_mi_ = mi;

	if (offensive_rg_parent_ && offensive_rg_img_ && offensive_rg_mi_)
	{
		offensive_rg_parent_->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}

	return false;
}

bool UPlayerStateUIBase::InitLClickGuideInstances(UWidget* parent_widget, UImage* img, UTextBlock* text)
{
	lclick_guide_parent_ = parent_widget;
	lclick_guide_img_ = img;
	lclick_guide_text_ = text;

	if (lclick_guide_parent_ && lclick_guide_img_ && lclick_guide_text_)
	{
		lclick_guide_parent_->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}

	return false;
}

bool UPlayerStateUIBase::InitRClickGuideInstances(UWidget* parent_widget, UImage* img, UTextBlock* text)
{
	rclick_guide_parent_ = parent_widget;
	rclick_guide_img_ = img;
	rclick_guide_text_ = text;

	if (rclick_guide_parent_ && rclick_guide_img_ && rclick_guide_text_)
	{
		rclick_guide_parent_->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}

	return false;
}



void UPlayerStateUIBase::SetStateUIVisible(EStateWidgets state_widget, bool visiblity)
{
	switch (state_widget)
	{
	case EStateWidgets::eStableRG:
		stable_rg_parent_->SetVisibility(visiblity ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	case EStateWidgets::eOffensiveRG:
		offensive_rg_parent_->SetVisibility(visiblity ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	case EStateWidgets::eLClickGuide:
		lclick_guide_parent_->SetVisibility(visiblity ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	case EStateWidgets::eRClickGuide:
		rclick_guide_parent_->SetVisibility(visiblity ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	}
}

void UPlayerStateUIBase::FillStableRG(float fill_value)
{
	if (stable_rg_activation_)
	{
		fill_value = std::min(fill_value, 1.0f);
		fill_value = std::max(fill_value, 0.0f);

		stable_rg_mi_->SetScalarParameterValue("AnimPhase", fill_value);
	}
}

void UPlayerStateUIBase::FillOffensiveRG(float fill_value)
{
	if (offensive_rg_activation_)
	{
		fill_value = std::min(fill_value, 1.0f);
		fill_value = std::max(fill_value, 0.0f);

		offensive_rg_mi_->SetScalarParameterValue("AnimPhase", fill_value);
	}
}

void UPlayerStateUIBase::LossStableRG()
{
	stable_rg_mi_->SetScalarParameterValue("AnimPhase", 0.0f);
}

void UPlayerStateUIBase::LossOffensiveRG()
{
	offensive_rg_mi_->SetScalarParameterValue("AnimPhase", 0.0f);
}

void UPlayerStateUIBase::ActiveLClickGuide(FText text)
{
	lclick_guide_text_->SetText(text);
}

void UPlayerStateUIBase::ActiveRClickGuide(FText text)
{
	rclick_guide_text_->SetText(text);
}

void UPlayerStateUIBase::RenderControl()
{
	//Visiblity Control
	stable_rg_img_->SetVisibility(stable_rg_parent_->GetVisibility());

	offensive_rg_img_->SetVisibility(offensive_rg_parent_->GetVisibility());

	lclick_guide_img_->SetVisibility(lclick_guide_parent_->GetVisibility());
	lclick_guide_text_->SetVisibility(lclick_guide_parent_->GetVisibility());

	rclick_guide_img_->SetVisibility(rclick_guide_parent_->GetVisibility());
	rclick_guide_text_->SetVisibility(rclick_guide_parent_->GetVisibility());
}
