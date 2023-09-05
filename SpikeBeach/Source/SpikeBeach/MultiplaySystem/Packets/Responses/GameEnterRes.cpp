// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEnterRes.h"
#include "../PacketId.h"

#include "../../../OpeningGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

GameEnterRes::GameEnterRes() : Packet(PacketId::GAME_ENTER_RES) {}

TArray<char> GameEnterRes::Serialize()
{
	packetLength = PACKET_SIZE + sizeof(errorCode);

	TArray<char> serialized;
	serialized.Reserve(packetLength);
	serialized.Append(Packet::Serialize());
	serialized.Append((const char*)errorCode, sizeof(errorCode));

	return serialized;
}

size_t GameEnterRes::Deserialize(char* buf, size_t length)
{
	size_t offset = Packet::Deserialize(buf, length);

	errorCode = *reinterpret_cast<uint16*>(buf + offset);
	offset += sizeof(errorCode);
	return offset;
}

void GameEnterRes::Process(UWorld* world)
{	
	//Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(world))->GameStartEvent();
}
