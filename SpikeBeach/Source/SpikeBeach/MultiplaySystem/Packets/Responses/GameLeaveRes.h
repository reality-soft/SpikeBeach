// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameLeaveRes : public Packet
{
public:
	uint16 errorCode;
	GameLeaveRes() : Packet(PacketId::GAME_LEAVE_RES) {}

	TArray<char> Serialize() override;
	size_t Deserialize(char* buf, size_t length) override;
};