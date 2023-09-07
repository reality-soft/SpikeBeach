// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API ControlReq : public Packet
{
public:
	float movementVector[2];

	ControlReq() : Packet(PacketId::CONTROLL_REQ) {};
	ControlReq(FVector2D movementVector) : Packet(PacketId::CONTROLL_REQ) {
		this->movementVector[0] = static_cast<float>(movementVector.X);
		this->movementVector[1] = static_cast<float>(movementVector.Y);
	};

	TArray<char> Serialize() override;
	size_t Deserialize(char* buf, size_t len) override;
};
