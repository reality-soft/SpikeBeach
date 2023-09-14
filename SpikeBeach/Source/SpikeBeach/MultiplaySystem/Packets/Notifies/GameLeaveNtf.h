// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameLeaveNtf : public Packet
{
public:
	INT16 userIdx;
	GameLeaveNtf() : Packet(PacketId::GAME_LEAVE_NTF) {}

	TArray<char> Serialize() override;
	size_t Deserialize(char* buf, size_t length) override;
};
