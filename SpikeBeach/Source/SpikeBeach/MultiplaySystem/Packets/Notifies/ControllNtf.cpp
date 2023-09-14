// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllNtf.h"

TArray<char> ControllNtf::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(userIdx) + sizeof(float) * 2;
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	serializeVec.Append(reinterpret_cast<char*>(userIdx), sizeof(INT16));
	serializeVec.Append(reinterpret_cast<char*>(movementVector), sizeof(float) * 2);

	return serializeVec;
}

size_t ControllNtf::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);

	userIdx = *reinterpret_cast<INT16*>(buf + offset);
	offset += sizeof(INT16);
	movementVector[0] = *reinterpret_cast<float*>(buf + offset);
	offset += sizeof(float);
	movementVector[1] = *reinterpret_cast<float*>(buf + offset);
	offset += sizeof(float);

    return size_t();
}
