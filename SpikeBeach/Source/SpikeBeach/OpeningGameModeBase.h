// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OpeningGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API AOpeningGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void EnterRoom(const FString& roomInfoString);

	UFUNCTION(BlueprintImplementableEvent)
		void ExitRoom();

	UFUNCTION(BlueprintImplementableEvent)
		void AddUserToRoom(const FString& enterUserNick);

	UFUNCTION(BlueprintImplementableEvent)
		void RemoveUserFromRoom(const FString& enterUserNick, const FString& newHostNick);
};
