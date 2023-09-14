// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameLeaveReq : public Packet
{
public:
	GameLeaveReq() : Packet(PacketId::GAME_LEAVE_REQ) {}
};