// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomEnterResponse.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomEnterResponse::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(ResponseHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterRes)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*roomInfoString), roomInfoString.Len() + 1);
    return Bytes;
}

int RoomEnterResponse::Deserialize(const TArray<uint8_t>& data)
{
    int Offset = ResponseHeader::Deserialize(data);
    roomInfoString = ReadString(data, Offset);

    return Offset + sizeof(int16);
}
