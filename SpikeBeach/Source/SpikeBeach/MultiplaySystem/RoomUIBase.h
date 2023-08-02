// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystemUIBase.h"
#include "IWebSocket.h"
#include "RoomUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API URoomUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	
public:
	TSharedPtr<IWebSocket> WebSocket;

	UFUNCTION(BlueprintCallable)
		void ConnectSocket();
};
