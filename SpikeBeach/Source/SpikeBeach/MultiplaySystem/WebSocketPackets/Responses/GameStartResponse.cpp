// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartResponse.h"
#include "../PacketIdDef.h"

TArray<uint8> GameStartResponse::Serialize()
{
    return ResponseHeader::Serialize(static_cast<int>(PacketIdDef::GameStartRes));
}

int GameStartResponse::Deserialize(const uint8* data)
{
    return ResponseHeader::Deserialize(data);
}
