// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLeaveNtf.h"

TArray<char> GameLeaveNtf::Serialize()
{
	TArray<char> serialized = Packet::Serialize();
	char* tempPtr = reinterpret_cast<char*>(&userIdx);
	serialized.Append(tempPtr, sizeof(INT16));
	return serialized;
}

size_t GameLeaveNtf::Deserialize(char* buf, size_t length)
{
	size_t offset = Packet::Deserialize(buf, length);
	if (offset == 0)
	{
		return 0;
	}
	else if (offset >= length)
	{
		return 0;
	}

	userIdx = *reinterpret_cast<INT16*>(buf + offset);
	offset += sizeof(INT16);
	return offset;
}
