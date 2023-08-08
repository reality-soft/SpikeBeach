// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomLeaveRequest.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomLeaveRequest::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(RequestHeader::Serialize(static_cast<int>(PacketIdDef::RoomLeaveReq)));
    return Bytes;
}

int RoomLeaveRequest::Deserialize(const uint8* data)
{
    int Offset = RequestHeader::Deserialize(data);
	return Offset + sizeof(int16);
}
