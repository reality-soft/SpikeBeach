// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotifyHeader.h"

/**
 * 
 */
class SPIKEBEACH_API GameStartNotify : public NotifyHeader
{
public:
    FString gameInfoString;
    FString ipAddress;
    int portNumber;

    TArray<uint8> Serialize();
    virtual int Deserialize(const uint8* data);
};
