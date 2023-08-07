// Fill out your copyright notice in the Description page of Project Settings.


#include "ResponseHeader.h"

TArray<uint8> ResponseHeader::Serialize(int fullPacketId)
{
    packetId = fullPacketId;
    TArray<uint8_t> data;
    data.Reset(sizeof(int));
    data.Append((const uint8*)&packetId, sizeof(int32));
    data.Append((const uint8*)&errorCode, sizeof(int16));
    return data;
}

int ResponseHeader::Deserialize(const TArray<uint8_t>& data)
{
    int offset = 0;
    std::memcpy(&packetId, data.GetData(), sizeof(int32));
    offset += sizeof(int32);

    std::memcpy(&packetId, data.GetData(), sizeof(int16));
    offset += sizeof(int16);

    return offset;
}

FString ResponseHeader::ReadString(const TArray<uint8_t>& data, int& offset)
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
