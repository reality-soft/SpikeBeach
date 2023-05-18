// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/SizeBox.h"

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
		bool InitReceiveRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		bool InitSpikeRGInstances(UImage* img, USizeBox* size, UWidgetAnimation* anim);



public:
	UImage* receive_rg_img_ = nullptr;
	USizeBox* receive_rg_size_ = nullptr;
	UWidgetAnimation* receive_rg_anim_ = nullptr;

	UImage* spike_rg_img_ = nullptr;
	USizeBox* spike_rg_size_ = nullptr;
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
