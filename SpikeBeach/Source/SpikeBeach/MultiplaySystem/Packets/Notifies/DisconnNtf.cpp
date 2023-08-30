// Fill out your copyright notice in the Description page of Project Settings.


#include "DisconnNtf.h"

TArray<char> DisconnNtf::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(playerIdx);

	TArray<char> serialized = Packet::Serialize();
	serialized.Reserve(packetLength);
	serialized.Append((const char*)playerIdx, sizeof(playerIdx));

	return serialized;
}

size_t DisconnNtf::Deserialize(char* buf, size_t length)
{
	size_t offset = Packet::Deserialize(buf, length);
	playerIdx = *reinterpret_cast<UINT16*>(buf + offset);
	offset += sizeof(playerIdx);
	return offset;
}