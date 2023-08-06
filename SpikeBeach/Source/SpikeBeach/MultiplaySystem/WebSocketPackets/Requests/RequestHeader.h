// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class PacketIdDef : uint8
{
    GenericError = 0,
    RoomEnterReq = 10,
    RoomEnterRes = 11,
    RoomEnterNtf = 12,
    RoomLeaveReq = 13,
    RoomLeaveRes = 14,
    RoomLeaveNtf = 15,
    RoomReadyReq = 16,
    RoomReadyRes = 17,
    RoomReadyNtf = 18,
    RoomUnreadyReq = 19,
    RoomUnreadyRes = 20,
    RoomUnreadyNtf = 21,
    GameStartReq = 22,
    GameStartRes = 23,
    GameStartNtf = 24,
};

/**
 * 
 */
class SPIKEBEACH_API URequestHeader
{
public:
    int packetId;

    TArray<uint8> Serialize(int fullPacketId);
    virtual int Deserialize(const TArray<uint8_t>& data);
    FString ReadString(const TArray<uint8_t>& data, int& offset);
};