// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "LoginSystemUIBase.generated.h"

UENUM(BlueprintType)
enum class EErrorCode : uint8
{
    // Generic Error : 0 ~ 49
    None = 0,
    AccountDbError = 1,
    SessionError = 2,
    RoomDbError = 3,
    GameDbError = 4,
    ServerError = 5,
    InvalidBodyForm = 6,
    InvalidPacketForm = 7,

    // Auth Error : 50 ~ 99
    InvalidId = 50,
    InvalidIdFormat = 51,
    DuplicatedId = 52,
    InvalidToken = 53,
    AreadyLogin = 54,
    WorngPassword = 55,
    WorngClientVersion = 56,

    // room Error : 100 ~
    RoomLeaveSuccess = 100,
    RoomNotExist = 101,
    RoomFull = 102,
    UserAlreadyInRoom = 103,
    UserNotInRoom = 104,
    UserAlreadyReady = 105,
    UserNotReady = 106
};

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API ULoginSystemUIBase : public UUserWidget
{
	GENERATED_BODY()

};
