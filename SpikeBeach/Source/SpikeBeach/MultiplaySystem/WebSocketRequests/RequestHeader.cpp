// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestHeader.h"

TArray<uint8> RequestHeader::Serialize(int fullPacketId)
{
    packetId = fullPacketId;
    TArray<uint8_t> data;
    data.Reset(sizeof(int));
    data.AddUninitialized(sizeof(int));
    return data;
}

int RequestHeader::Deserialize(const std::vector<uint8_t>& data)
{
    std::memcpy(&packetId, data.data(), sizeof(int));
    return sizeof(int);
}

FString RequestHeader::ReadString(const std::vector<uint8_t>& data, int& offset)
{
    int length = 0;
    while (offset + length < static_cast<int>(data.size()) && data[offset + length] != '\0')
    {
        length++;
    }

    FString str(reinterpret_cast<const char*>(data.data() + offset), length);
    offset += length + 1;
    return str;
}