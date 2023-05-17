// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomPlayerController.h"
#include "PlayerStateUIBase.h"

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

public:
	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float receive_proficiency = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float spike_proficiency = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "UISystem")
		float player_stamina = 1.0f;

	UCapsuleComponent* player_capsule_ = nullptr;
	ACustomPlayerController* player_controller_ = nullptr;
	UPlayerStateUIBase* player_state_ui_ = nullptr;

public:	
	UFUNCTION(BlueprintCallable, Category = "UISystem")
		bool InitInstances(UCapsuleComponent* player_calsule_, ACustomPlayerController* player_controller, UPlayerStateUIBase* state_ui);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
