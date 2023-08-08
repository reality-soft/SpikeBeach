// Fill out your copyright notice in the Description page of Project Settings.


#include "ResponseHeader.h"
#include "Containers/UnrealString.h"

TArray<uint8> ResponseHeader::Serialize(int fullPacketId)
{
    packetId = fullPacketId;
    TArray<uint8_t> data;
    data.Reset(sizeof(int));
    data.Append((const uint8*)&packetId, sizeof(int32));
    data.Append((const uint8*)&errorCode, sizeof(int16));
    return data;
}

int ResponseHeader::Deserialize(const uint8* data)
{
    int offset = 0;
    std::memcpy(&packetId, data, sizeof(int32));
    offset += sizeof(int32);

    std::memcpy(&errorCode, data + offset, sizeof(int16));
    offset += sizeof(int16);

    return offset;
}

FString ResponseHeader::ReadString(const uint8* data, int& offset)
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
