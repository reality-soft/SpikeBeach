// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RequestHeader.h"

/**
 * 
 */
class SPIKEBEACH_API URoomEnterRequest : public URequestHeader
{
public:
    FString userAssignedId;
    FString token;
    FString clientVersion;
    int16 roomId;

    TArray<uint8> Serialize();
    virtual int Deserialize(const TArray<uint8_t>& data);
};
