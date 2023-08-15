// Fill out your copyright notice in the Description page of Project Settings.


#include "UnreadyRequest.h"
#include "../PacketIdDef.h"

TArray<uint8> UnreadyRequest::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(RequestHeader::Serialize(static_cast<int>(PacketIdDef::RoomUnreadyReq)));
    return Bytes;
}

int UnreadyRequest::Deserialize(const uint8* data)
{
    int Offset = RequestHeader::Deserialize(data);

    return Offset + sizeof(int16);
}
