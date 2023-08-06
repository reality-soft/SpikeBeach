// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomEnterRequest.h"
#include "RequestHeader.h"

TArray<uint8> URoomEnterRequest::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(URequestHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterReq))); // Make sure to call the base class's Serialize method
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*userAssignedId), userAssignedId.Len() + 1);
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*token), token.Len() + 1);
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*clientVersion), clientVersion.Len() + 1);
    Bytes.Append((const uint8*)&roomId, sizeof(int32));
    return Bytes;
}

int URoomEnterRequest::Deserialize(const TArray<uint8_t>& data)
{
    int Offset = URequestHeader::Deserialize(data); // Make sure to call the base class's Deserialize method
    userAssignedId = ReadString(data, Offset);
    Offset += userAssignedId.Len() + 1;
    token = ReadString(data, Offset);
    Offset += token.Len() + 1;

    clientVersion = ReadString(data, Offset);
    Offset += clientVersion.Len() + 1;

    FMemory::Memcpy(&roomId, data.GetData() + Offset, sizeof(int16));

    return Offset + sizeof(int16);
}
