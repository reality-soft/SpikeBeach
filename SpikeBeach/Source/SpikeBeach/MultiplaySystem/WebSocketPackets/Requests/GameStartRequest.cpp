// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartRequest.h"
#include "../PacketIdDef.h"

TArray<uint8> GameStartRequest::Serialize()
{
    return RequestHeader::Serialize(static_cast<int>(PacketIdDef::GameStartReq));
}

int GameStartRequest::Deserialize(const uint8* data)
{
    return RequestHeader::Deserialize(data);
}
