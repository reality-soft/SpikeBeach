// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyRequest.h"
#include "../PacketIdDef.h"

TArray<uint8> ReadyRequest::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(RequestHeader::Serialize(static_cast<int>(PacketIdDef::RoomReadyReq)));
    Bytes.Append((const uint8*)&team, sizeof(int16));
    return Bytes;
}

int ReadyRequest::Deserialize(const uint8* data)
{
    int Offset = RequestHeader::Deserialize(data);

    FMemory::Memcpy(&team, data + Offset, sizeof(int16));
    Offset += sizeof(int16);

    return Offset + sizeof(int16);
}
