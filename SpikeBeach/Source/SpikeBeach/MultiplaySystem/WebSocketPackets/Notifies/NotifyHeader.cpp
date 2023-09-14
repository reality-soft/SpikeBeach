// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyHeader.h"

TArray<uint8> NotifyHeader::Serialize(int fullPacketId)
{
    packetId = fullPacketId;
    TArray<uint8_t> data;
    data.Reset(sizeof(int));
    data.Append((const uint8*)&packetId, sizeof(int32));
    return data;
}

int NotifyHeader::Deserialize(const uint8* data)
{
    std::memcpy(&packetId, data, sizeof(int32));
    return sizeof(int32);
}

FString NotifyHeader::ReadString(const uint8* data, int& offset)
{
    int length = 0;
    TArray <uint8> strringArray;
    while (data[offset + length] != '\0')
    {
        length++;
    }
    
    FString str(reinterpret_cast<const char*>(data + offset));

    offset += length + 1;
    return str;
}