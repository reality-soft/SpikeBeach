// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API DisconnNtf : public Packet
{
public:
	INT16 playerIdx;
	DisconnNtf() : Packet(PacketId::DISCONN_NTF) {}
	TArray<char> Serialize() override;
	size_t Deserialize(char* buf, size_t length) override;
};
