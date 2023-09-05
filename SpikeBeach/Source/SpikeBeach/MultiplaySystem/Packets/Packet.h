// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketId.h"

#define PACKET_SIZE 2 * sizeof(INT32)

/**
 * 
 */
class SPIKEBEACH_API Packet
{
public:
	INT32 packetLength;
	INT32 packetId;

	Packet(INT32 packetId) : packetId(packetId) {}
	virtual TArray<char> Serialize();
	virtual size_t Deserialize(char* buf, size_t length);
	FString ReadString(const char* data, size_t& offset);

	virtual void Process() {};

	virtual ~Packet();

public:
	static INT32 ParsePacketLength(char* base, size_t bufSize)
	{
		if (bufSize < sizeof(packetLength))
		{
			return 0;
		}
		INT32 packetLength = *reinterpret_cast<INT32*>(base);
		return packetLength;
	}

	static PacketId GetPacketId(char* base, size_t bufSize)
	{
		if (bufSize < sizeof(packetLength) + sizeof(packetId))
		{
			return PacketId::ERROR_OCCUR;
		}
		INT32 packetId = *reinterpret_cast<INT32*>(base + sizeof(packetLength));
		return (PacketId)packetId;
	}
};
