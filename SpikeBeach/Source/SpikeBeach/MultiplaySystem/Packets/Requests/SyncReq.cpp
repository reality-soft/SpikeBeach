// Fill out your copyright notice in the Description page of Project Settings.


#include "SyncReq.h"
#include <chrono>

TArray<char> SyncReq::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(syncReqTime) + sizeof(RTT);
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	syncReqTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	serializeVec.Append(reinterpret_cast<char*>(&syncReqTime), sizeof(syncReqTime));
	serializeVec.Append(reinterpret_cast<char*>(&RTT), sizeof(RTT));
	return serializeVec;
}

size_t SyncReq::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);

	std::copy(buf + offset, buf + offset + sizeof(syncReqTime), &syncReqTime);
	offset += sizeof(syncReqTime);
	std::copy(buf + offset, buf + offset + sizeof(RTT), &RTT);
	offset += sizeof(RTT);
	return offset;
}