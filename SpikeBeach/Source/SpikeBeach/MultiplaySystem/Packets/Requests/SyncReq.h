// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API SyncReq : public Packet
{
public:
	SyncReq() : Packet(PacketId::SYNC_REQ) {}
	INT64 syncReqTime;
	INT64 RTT;

	virtual TArray<char> Serialize() override;

	virtual size_t Deserialize(char* buf, size_t len) override;
};