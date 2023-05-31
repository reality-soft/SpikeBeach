// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/SizeBox.h"
#include "UMG/Public/Components/TextBlock.h"

#include "PlayerStateUIBase.generated.h"

/**
 * 
 */
UENUM()
enum class EStateWidgets
{
	eStableRG,
	eOffensiveRG,
	eLClickGuide,
	eRClickGuide
};

UCLASS()
class SPIKEBEACH_API UPlayerStateUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Stable Gauge")
		bool InitReceiveRGInstances(UWidget* parent_widget, UImage* img, UMaterialInstanceDynamic* mi);

	UFUNCTION(BlueprintCallable, Category = "Offensive Gauge")
		bool InitSpikeRGInstances(UWidget* parent_widget, UImage* img, UMaterialInstanceDynamic* mi);

	UFUNCTION(BlueprintCallable, Category = "Controll Guide")
		bool InitLClickGuideInstances(UWidget* parent_widget, UImage* img, UTextBlock* text);

	UFUNCTION(BlueprintCallable, Category = "Controll Guide")
		bool InitRClickGuideInstances(UWidget* parent_widget, UImage* img, UTextBlock* text);

public:
	// Gauges
	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		UWidget* stable_rg_parent_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		UImage* stable_rg_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Stable Gauge")
		UMaterialInstanceDynamic* stable_rg_mi_ = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		UWidget* offensive_rg_parent_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		UImage* offensive_rg_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Offensive Gauge")
		UMaterialInstanceDynamic* offensive_rg_mi_ = nullptr;


	// Controll Guide
	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UWidget* lclick_guide_parent_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UImage* lclick_guide_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UTextBlock* lclick_guide_text_ = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UWidget* rclick_guide_parent_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UImage* rclick_guide_img_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controll Guide")
		UTextBlock* rclick_guide_text_ = nullptr;

	// Ping Cursor
	UPROPERTY(BlueprintReadWrite, Category = "Ping")
		UImage* ping_cursor_img_ = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
		void SetStateUIVisible(EStateWidgets state_widget, bool visiblity);

	UFUNCTION(BlueprintCallable, Category = "Render Control")
		void RenderControl();

	UFUNCTION(BlueprintCallable, Category = "Stable Gauge")
		void FillStableRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Offensive Gauge")
		void FillOffensiveRG(float fill_value);

	UFUNCTION(BlueprintCallable, Category = "Stable Gauge")
		void LossStableRG();

	UFUNCTION(BlueprintCallable, Category = "Offensive Gauge")
		void LossOffensiveRG();

	UFUNCTION(BlueprintCallable, Category = "Control Guide")
		void ActiveLClickGuide(FText text);

	UFUNCTION(BlueprintCallable, Category = "Control Guide")
		void ActiveRClickGuide(FText text);

public:
	bool stable_rg_activation_ = false;
	bool offensive_rg_activation_ = false;
};
