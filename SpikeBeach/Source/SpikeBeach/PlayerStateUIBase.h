// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Image.h"

#include "PlayerStateUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API UPlayerStateUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
		bool InitRGImages(UImage* receive_rg_img, UImage* spike_rg_img, UWidgetAnimation* receive_rg_anim, UWidgetAnimation* spike_rg_anim);

public:
	UImage* receive_rg_img_ = nullptr;
	UImage* spike_rg_img_ = nullptr;
	UWidgetAnimation* receive_rg_anim_ = nullptr;
	UWidgetAnimation* spike_rg_anim_ = nullptr;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		float rg_1_animtime_ = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		float rg_2_animtime_ = 0.0f;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
		void FillReceiveRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void FillSpikeRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void LossSpikeRG();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void LossReceiveRG();
};
