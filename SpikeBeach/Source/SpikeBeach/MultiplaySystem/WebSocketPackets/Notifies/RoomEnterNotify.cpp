// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomEnterNotify.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomEnterNotify::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(NotifyHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterNtf)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*enterUserNick), enterUserNick.Len() + 1);
    return Bytes;
}

int RoomEnterNotify::Deserialize(const uint8* data)
{
    int Offset = NotifyHeader::Deserialize(data);
    enterUserNick = ReadString(data, Offset);
    Offset += enterUserNick.Len() + 1;

    return Offset + sizeof(int16);
}
