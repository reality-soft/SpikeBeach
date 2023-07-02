// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystem/LoginSystemUIBase.h"
#include "LoginUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API ULoginUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void LoginRequest(FString userAssignedId, FString password, FString clientVersion);

	void OnLoginResponseRecevied(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
