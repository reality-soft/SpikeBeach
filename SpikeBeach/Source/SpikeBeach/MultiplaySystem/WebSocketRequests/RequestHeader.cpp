// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestHeader.h"

TArray<uint8> URequestHeader::Serialize(int fullPacketId)
{
    packetId = fullPacketId;
    TArray<uint8_t> data;
    data.Reset(sizeof(int));
    data.Append((const uint8*)&packetId, sizeof(int32));
    return data;
}

int URequestHeader::Deserialize(const TArray<uint8_t>& data)
{
    std::memcpy(&packetId, data.GetData(), sizeof(int));
    return sizeof(int);
}

FString URequestHeader::ReadString(const TArray<uint8_t>& data, int& offset)
{
    int length = 0;
    while (offset + length < static_cast<int>(data.Num()) && data[offset + length] != '\0')
    {
        length++;
    }

    FString str(reinterpret_cast<const char*>(data.Num() + offset), length);
    offset += length + 1;
    return str;
}