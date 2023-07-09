// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystemUIBase.h"
#include "RegisterUIBase.generated.h"

UENUM(BlueprintType)
enum class ERegisterState : uint8 {
	RS_NONE,
	RS_INVALID_ID,
	RS_INVALID_ID_FORMAT,
	RS_DUPLICATED_ID,
};

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API URegisterUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void RegisterRequest(FString userAssignedId, FString nickname, FString password);

	void OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintImplementableEvent)
		void OnSuccess();

	UPROPERTY(BlueprintReadWrite, Category = Location, meta = (AllowPrivateAccess = "true"))
		ERegisterState RegisterState = ERegisterState::RS_NONE;
};
