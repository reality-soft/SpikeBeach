// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomLeaveNotify.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomLeaveNotify::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(NotifyHeader::Serialize(static_cast<int>(PacketIdDef::RoomLeaveNtf)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*leaveInfoString), leaveInfoString.Len() + 1);
    return Bytes;
}

int RoomLeaveNotify::Deserialize(const uint8* data)
{
    int Offset = NotifyHeader::Deserialize(data);
    leaveInfoString = ReadString(data, Offset);
    Offset += leaveInfoString.Len() + 1;

    return Offset + sizeof(int16);
}
