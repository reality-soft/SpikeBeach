// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

/**
 * 
 */
class SPIKEBEACH_API GameStartNtf : public Packet
{
public:
	GameStartNtf() : Packet(PacketId::GAME_START_NTF) {}
	INT64 gameStartTime;
	TArray<FString> nickNames; // userIdx : 1(red 1p), 2(red 2p), 3(blue 1p), 4(blue 2p)

	TArray<char> Serialize() override;

	size_t Deserialize(char* buf, size_t length) override;

};
