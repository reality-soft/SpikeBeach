// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPIKEBEACH_API RequestHeader
{
public:
    int packetId;

    TArray<uint8> Serialize(int fullPacketId);
    virtual int Deserialize(const uint8* data);
    FString ReadString(const uint8* data, int& offset);
};