// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyNotify.h"
#include "../PacketIdDef.h"

TArray<uint8> ReadyNotify::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(NotifyHeader::Serialize(static_cast<int>(PacketIdDef::RoomReadyNtf)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*teamString), teamString.Len() + 1);
    return Bytes;
}

int ReadyNotify::Deserialize(const uint8* data)
{
    int Offset = NotifyHeader::Deserialize(data);
    teamString = ReadString(data, Offset);
    Offset += teamString.Len() + 1;

    return Offset + sizeof(int16);
}
