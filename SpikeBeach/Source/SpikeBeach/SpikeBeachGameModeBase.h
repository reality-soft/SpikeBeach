// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpikeBeachGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API ASpikeBeachGameModeBase : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
