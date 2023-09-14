// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystemUIBase.h"
#include "UserInRoom.h"
#include "RoomUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API URoomUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomInfo")
		TArray<FUserInRoom> userInfo_;

public:
	UFUNCTION(BlueprintCallable)
		void ExitRoom();

	UFUNCTION(BlueprintCallable)
		void OnNotifyOrResponseRecevied(const TArray<FUserInRoom>& userInfo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RefreshUserList();
};
