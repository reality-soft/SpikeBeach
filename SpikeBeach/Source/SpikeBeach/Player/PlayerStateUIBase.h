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
	UFUNCTION(BlueprintCallable, Category = "Stable Gauge")
		bool InitReceiveRGInstances(UImage* img, USizeBox* size, UMaterialInstanceDynamic* mi);

	UFUNCTION(BlueprintCallable, Category = "Offensive Gauge")
		bool InitSpikeRGInstances(UImage* img, USizeBox* size, UMaterialInstanceDynamic* mi);

protected:
	FTimerHandle timer_handle_;
	FTimerManager timer_manager_;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		UImage* stable_rg_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		USizeBox* stable_rg_size_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		UMaterialInstanceDynamic* stable_rg_mi_ = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		UImage* offensive_rg_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		USizeBox* offensive_rg_size_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		UMaterialInstanceDynamic* offensive_rg_mi_ = nullptr;

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

public:


};
