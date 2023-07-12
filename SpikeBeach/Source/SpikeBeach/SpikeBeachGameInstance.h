// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "SpikeBeachGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API USpikeBeachGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Token")
		FString login_token_;

	virtual void Init() override;
	virtual void Shutdown() override;

	TSharedPtr<IWebSocket> WebSocket;
	
	UFUNCTION(BlueprintCallable)
		void SendMessage();
};
