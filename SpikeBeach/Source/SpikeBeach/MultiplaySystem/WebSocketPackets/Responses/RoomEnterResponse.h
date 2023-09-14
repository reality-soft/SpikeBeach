// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResponseHeader.h"
#include "../../UserInRoom.h"

/**
 * 
 */
class SPIKEBEACH_API RoomEnterResponse : public ResponseHeader
{
public:
    FString roomInfoString;
    
    int roomNum;
    FString roomName;

    TArray<uint8> Serialize();
    virtual int Deserialize(const uint8* data);

    void ProcessUserInfo(TArray<FUserInRoom>& userInfo);
};
