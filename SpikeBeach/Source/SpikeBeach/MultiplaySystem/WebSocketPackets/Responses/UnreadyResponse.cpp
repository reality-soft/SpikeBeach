// Fill out your copyright notice in the Description page of Project Settings.


#include "UnreadyResponse.h"
#include "../PacketIdDef.h"

TArray<uint8> UnreadyResponse::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(ResponseHeader::Serialize(static_cast<int>(PacketIdDef::RoomUnreadyRes)));
    return Bytes;
}

int UnreadyResponse::Deserialize(const uint8* data)
{
    int Offset = ResponseHeader::Deserialize(data);

    return Offset + sizeof(int16);
}
