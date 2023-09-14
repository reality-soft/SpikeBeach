// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API DisconnRes : public Packet
{
public:
	DisconnRes() : Packet(PacketId::DISCONN_RES) {}
};
