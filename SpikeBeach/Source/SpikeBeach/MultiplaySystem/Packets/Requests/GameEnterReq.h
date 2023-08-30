// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameEnterReq : public Packet
{
public:
	GameEnterReq() : Packet(PacketId::GAME_ENTER_REQ) {}
	FString userAssignedId;
	FString token;
	FString clientVersion;
	INT32 gameId; //roomId

	size_t Deserialize(char* buf, size_t length);
	TArray<char> Serialize();
};
