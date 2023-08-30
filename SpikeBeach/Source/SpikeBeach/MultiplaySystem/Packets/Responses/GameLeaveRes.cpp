// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLeaveRes.h"

TArray<char> GameLeaveRes::Serialize()
{
	TArray<char> serialized = Packet::Serialize();
	char* tempPtr = reinterpret_cast<char*>(&errorCode);
	serialized.Append(tempPtr, sizeof(errorCode));
	return serialized;
}

size_t GameLeaveRes::Deserialize(char* buf, size_t length)
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
	errorCode = *reinterpret_cast<uint16*>(buf + offset);
	offset += sizeof(uint16);
	return offset;
};
