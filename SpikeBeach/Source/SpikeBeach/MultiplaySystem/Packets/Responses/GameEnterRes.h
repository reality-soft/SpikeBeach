// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameEnterRes : public Packet
{
public:
	GameEnterRes();
	int16 errorCode;

public:
	TArray<char> Serialize();
	size_t Deserialize(char* buf, size_t length);
};
