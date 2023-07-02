// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystem/LoginSystemUIBase.h"
#include "RegisterUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API URegisterUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
		void RegisterRequest(FString userAssignedId, FString password, FString clientVersion);

	void OnRegisterResponseRecevied(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
