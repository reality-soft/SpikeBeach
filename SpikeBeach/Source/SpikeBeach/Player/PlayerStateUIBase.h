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

protected:
	FTimerHandle timer_handle_;
	FTimerManager timer_manager_;

public:
	UImage* stable_rg_img_ = nullptr;
	USizeBox* stable_rg_size_ = nullptr;
	UWidgetAnimation* stable_rg_anim_ = nullptr;

	UImage* offensive_rg_img_ = nullptr;
	USizeBox* offensive_rg_size_ = nullptr;
	UWidgetAnimation* offensive_rg_anim_ = nullptr;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		float rg_1_animtime_ = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		float rg_2_animtime_ = 0.0f;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
		void FillStableRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void LossStableRG();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void UnshowStableRG();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void FillOffensiveRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void LossOffensiveRG();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void UnshowOffensiveRG();
};
