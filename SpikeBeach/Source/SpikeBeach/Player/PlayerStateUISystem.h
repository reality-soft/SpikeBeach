// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomPlayerController.h"
#include "PlayerStateUIBase.h"
#include "BasePlayer.h"
#include "Blueprint/UserWidget.h"

#include "PlayerStateUISystem.generated.h"

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPIKEBEACH_API UPlayerStateUISystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateUISystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle timer_handle_;
	FTimerManager timer_manager_;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Widget")
		FVector2D player_screen_pos_;

	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float receive_proficiency = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float spike_proficiency = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float player_stamina = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Instance")
		ABasePlayer* target_player_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Instance")
		UCapsuleComponent* player_capsule_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Instance")
		ACustomPlayerController* player_controller_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Instance")
		UPlayerStateUIBase* player_state_ui_ = nullptr;

public:	
	UFUNCTION(BlueprintCallable, Category = "UISystem")
		bool InitInstances(ABasePlayer* player_, UCapsuleComponent* player_calsule_, ACustomPlayerController* player_controller, UPlayerStateUIBase* state_ui);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector2D receive_rg_local_ = { -150, -150 };
	FVector2D spike_rg_local_ = { -150, -150 };
};
