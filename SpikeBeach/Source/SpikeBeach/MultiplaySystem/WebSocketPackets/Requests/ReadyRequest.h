// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RequestHeader.h"

enum Team : int16
{
	teamA = 0,
	teamB = 1,
};


/**
 * 
 */
class SPIKEBEACH_API ReadyRequest : public RequestHeader
{
public:
	Team team;

	TArray<uint8> Serialize();
	virtual int Deserialize(const uint8* data);
};
