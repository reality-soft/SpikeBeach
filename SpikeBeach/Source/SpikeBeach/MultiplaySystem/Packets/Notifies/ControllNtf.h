// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API ControllNtf : public Packet
{
public:
	INT16 userIdx;
	float movementVector[2];

	ControllNtf() : Packet(PacketId::CONTROLL_REQ) {};

	float movementVector[2];

	TArray<char> Serialize() override;
	size_t Deserialize(char* buf, size_t len) override;
};
