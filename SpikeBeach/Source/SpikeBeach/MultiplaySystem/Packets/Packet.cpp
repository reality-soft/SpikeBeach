// Fill out your copyright notice in the Description page of Project Settings.


#include "Packet.h"

TArray<char> Packet::Serialize()
{
	TArray<char> serialized;
	serialized.Append((const char*)&packetLength, sizeof(INT32));
	serialized.Append((const char*)&packetId, sizeof(INT32));
	return serialized;
}

size_t Packet::Deserialize(char* buf, size_t length)
{
	size_t totalSize = sizeof(packetLength) + sizeof(packetId);
	if (length < totalSize)
	{
		packetId = PacketId::ERROR_OCCUR;
		return 0;
	}

	packetLength = *reinterpret_cast<INT32*>(buf);
	packetId = *reinterpret_cast<INT32*>(buf + sizeof(packetLength));
	return totalSize;
}
FString Packet::ReadString(const char* data, size_t& offset)
{
	int length = 0;
	TArray <char> strringArray;
	while (data[offset + length] != '\0')
	{
		length++;
	}

	FString str(reinterpret_cast<const char*>(data + offset));

	offset += length + 1;
	return str;
}
