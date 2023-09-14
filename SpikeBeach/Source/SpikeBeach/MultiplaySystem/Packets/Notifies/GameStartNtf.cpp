// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartNtf.h"


TArray<char> GameStartNtf::Serialize()
{
	Packet::packetLength = PACKET_SIZE + sizeof(gameStartTime);

	for (size_t i = 0; i < nickNames.Num(); i++)
	{
		Packet::packetLength += nickNames[i].Len() + 1;
	}

	TArray<char> serialized = Packet::Serialize();
	serialized.Reserve(packetLength);

	char* tempPtr = reinterpret_cast<char*>(&gameStartTime);
	serialized.Append(tempPtr, sizeof(gameStartTime));
	for (auto& nickName : nickNames)
	{
		serialized.Append((const char *)(*nickName), nickName.Len() + 1);
	}

	return serialized;
}

size_t GameStartNtf::Deserialize(char* buf, size_t length)
{
	size_t offset = Packet::Deserialize(buf, length);
	gameStartTime = *reinterpret_cast<INT64*>(buf + offset);
	offset += sizeof(gameStartTime);
	for (auto& nickName : nickNames)
	{
		nickName = ReadString(buf, offset);
		offset += nickName.Len() + 1;
	}

	return offset;
}