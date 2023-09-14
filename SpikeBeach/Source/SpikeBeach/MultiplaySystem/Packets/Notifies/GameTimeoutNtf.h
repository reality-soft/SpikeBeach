// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameTimeoutNtf : public Packet
{
public:
	GameTimeoutNtf() : Packet(PacketId::GAME_TIMEOUT_NTF) {}
};
