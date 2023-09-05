// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplaySystem/UserInRoom.h"
#include "OpeningGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API AOpeningGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		TArray<FUserInRoom> userInfo_;

	UFUNCTION(BlueprintImplementableEvent)
		void EnterRoom();

	UFUNCTION(BlueprintImplementableEvent)
		void ExitRoom();

	UFUNCTION(BlueprintImplementableEvent)
		void RefreshUserInfo();

	UFUNCTION(BlueprintImplementableEvent)
		void GameStartEvent();
};
