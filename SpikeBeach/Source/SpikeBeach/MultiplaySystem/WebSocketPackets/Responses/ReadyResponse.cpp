// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyResponse.h"
#include "../PacketIdDef.h"


TArray<uint8> ReadyResponse::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(ResponseHeader::Serialize(static_cast<int>(PacketIdDef::RoomReadyRes)));
    return Bytes;
}

int ReadyResponse::Deserialize(const uint8* data)
{
    return ResponseHeader::Deserialize(data);
}
