// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotifyHeader.h"
#include "../../UserInRoom.h"

/**
 * 
 */
class SPIKEBEACH_API RoomEnterNotify : public NotifyHeader
{
public:
    FString enterUserNick;

    TArray<uint8> Serialize();
    virtual int Deserialize(const uint8* data);

    void ProcessUserInfo(TArray<FUserInRoom>& userInfo);
};
