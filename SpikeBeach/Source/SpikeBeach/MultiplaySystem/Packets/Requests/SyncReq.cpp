// Fill out your copyright notice in the Description page of Project Settings.


#include "SyncReq.h"

TArray<char> SyncReq::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(syncReqTime);
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	serializeVec.Append(reinterpret_cast<char*>(&syncReqTime), sizeof(syncReqTime));
	return serializeVec;
}

size_t SyncReq::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);

	std::copy(buf + offset, buf + offset + sizeof(syncReqTime), &syncReqTime);
	offset += sizeof(syncReqTime);
	return offset;
}