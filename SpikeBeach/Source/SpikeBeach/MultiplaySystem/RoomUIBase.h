// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystemUIBase.h"
#include "RoomUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API URoomUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void ExitRoom();

};
