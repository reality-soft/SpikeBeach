// Fill out your copyright notice in the Description page of Project Settings.


#include "SyncRes.h"

TArray<char> SyncRes::Serialize()
{
	packetLength = PACKET_SIZE + USER_SYNC_DATA_SIZE;
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	serializeVec.Append(reinterpret_cast<char*>(&syncTime), sizeof(syncTime));
	serializeVec.Append(reinterpret_cast<char*>(&latency), sizeof(latency));

	for (size_t i = 0; i < 4; i++)
	{
		serializeVec.Append(users[i].Serialize());
	}

	return serializeVec;
}

size_t SyncRes::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);
	users.Empty();
	users.SetNum(4);

	std::copy(buf + offset, buf + sizeof(syncTime), &syncTime);
	offset += sizeof(syncTime);
	std::copy(buf + offset, buf + sizeof(latency), reinterpret_cast<char*>(&latency));
	offset += sizeof(latency);

	for (size_t i = 0; i < 4; i++)
	{
		offset += users[i].Deserialize(buf + offset, len - offset);
	}

	return offset;
}
