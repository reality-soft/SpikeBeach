// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEnterReq.h"

size_t GameEnterReq::Deserialize(char* buf, size_t length)
{
	size_t offset = Packet::Deserialize(buf, length);

	userAssignedId = FString(buf + offset);
	offset += userAssignedId.Len() + 1; // +1 to skip the '\0' delimiter

	// Parse token
	token = FString(buf + offset);
	offset += token.Len() + 1;

	// Parse clientVersion
	clientVersion = FString(buf + offset);
	offset += clientVersion.Len() + 1;

	// Parse gameId
	gameId = *reinterpret_cast<INT32*>(buf + offset);
	offset += sizeof(gameId);

	return offset;
}

TArray<char> GameEnterReq::Serialize()
{
	packetId = PacketId::GAME_ENTER_REQ;
	packetLength = PACKET_SIZE + userAssignedId.Len() + token.Len() + clientVersion.Len() + 3 + sizeof(gameId);
	TArray<char> serialized = Packet::Serialize();
	serialized.Reserve(packetLength);

	serialized.Append((const char*)(*userAssignedId), userAssignedId.Len() + 1);
	serialized.Append((const char*)(*token), token.Len() + 1);
	serialized.Append((const char*)(*clientVersion), clientVersion.Len() + 1);
	serialized.Append((const char*)&gameId, sizeof(gameId));

	return serialized;
}
