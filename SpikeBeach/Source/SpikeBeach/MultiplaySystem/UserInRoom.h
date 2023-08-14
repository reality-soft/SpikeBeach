// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserInRoom.generated.h"

UENUM(BlueprintType)
enum class EReadyState : uint8
{
	eNotReady,
	eATeam,
	eBTeam
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct SPIKEBEACH_API FUserInRoom
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString nickName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyInfo")
		EReadyState readyState = EReadyState::eNotReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HostInfo")
		bool isHost = false;
};
