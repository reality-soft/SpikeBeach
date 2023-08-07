// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomEnterRequest.h"
#include "RequestHeader.h"
#include "../PacketIdDef.h"

TArray<uint8> URoomEnterRequest::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(RequestHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterReq)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*userAssignedId), userAssignedId.Len() + 1);
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*token), token.Len() + 1);
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*clientVersion), clientVersion.Len() + 1);
    Bytes.Append((const uint8*)&roomId, sizeof(int32));
    return Bytes;
}

int URoomEnterRequest::Deserialize(const TArray<uint8_t>& data)
{
    int Offset = RequestHeader::Deserialize(data);
    userAssignedId = ReadString(data, Offset);
    Offset += userAssignedId.Len() + 1;
    token = ReadString(data, Offset);
    Offset += token.Len() + 1;

    clientVersion = ReadString(data, Offset);
    Offset += clientVersion.Len() + 1;

    FMemory::Memcpy(&roomId, data.GetData() + Offset, sizeof(int16));

    return Offset + sizeof(int16);
}
