// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomLeaveResponse.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomLeaveResponse::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(ResponseHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterRes)));
    return Bytes;
}

int RoomLeaveResponse::Deserialize(const uint8* data)
{
    int Offset = ResponseHeader::Deserialize(data);
    return Offset + sizeof(int16);
}
