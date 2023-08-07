// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPIKEBEACH_API ResponseHeader
{
public:
	int packetId;
	int16 errorCode;

	TArray<uint8> Serialize(int fullPacketId);
	virtual int Deserialize(const TArray<uint8_t>& data);
	FString ReadString(const TArray<uint8_t>& data, int& offset);
};
