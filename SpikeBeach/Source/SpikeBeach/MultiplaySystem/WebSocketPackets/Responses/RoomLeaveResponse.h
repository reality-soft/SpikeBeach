// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResponseHeader.h"

/**
 * 
 */
class SPIKEBEACH_API RoomLeaveResponse : public ResponseHeader
{
public:
	TArray<uint8> Serialize();
	virtual int Deserialize(const uint8* data);
};
