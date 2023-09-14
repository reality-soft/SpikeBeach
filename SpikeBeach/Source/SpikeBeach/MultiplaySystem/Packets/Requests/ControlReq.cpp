// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlReq.h"

TArray<char> ControlReq::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(float) * 2;
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	serializeVec.Append(reinterpret_cast<char*>(movementVector), sizeof(float) * 2);

	return serializeVec;
}

size_t ControlReq::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);

	movementVector[0] = *reinterpret_cast<float*>(buf + offset);
	offset += sizeof(float);
	movementVector[1] = *reinterpret_cast<float*>(buf + offset);
	offset += sizeof(float);

	return offset;
}
