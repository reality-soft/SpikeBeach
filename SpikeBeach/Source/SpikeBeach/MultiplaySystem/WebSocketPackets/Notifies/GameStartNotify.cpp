// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartNotify.h"
#include "../PacketIdDef.h"

TArray<uint8> GameStartNotify::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(NotifyHeader::Serialize(static_cast<int>(PacketIdDef::GameStartNtf)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*gameInfoString), gameInfoString.Len() + 1);
    return Bytes;
}

int GameStartNotify::Deserialize(const uint8* data)
{
    int Offset = NotifyHeader::Deserialize(data);
    gameInfoString = ReadString(data, Offset);

    TArray<FString> info;
    gameInfoString.Mid(2).ParseIntoArray(info, TEXT(":"), true);
    ipAddress = info[0];
    portNumber = FCString::Atoi(*info[1]);

    Offset += gameInfoString.Len() + 1;

    return Offset + sizeof(int16);;
}
