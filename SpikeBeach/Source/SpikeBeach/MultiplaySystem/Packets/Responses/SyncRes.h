// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Packet.h"

#define POS_SIZE 3 * sizeof(float)
#define VEL_SIZE 3 * sizeof(float)
#define ACC_SIZE 3 * sizeof(float)
#define SYNC_INFO_SIZE 5 * sizeof(INT64) + 4 * (POS_SIZE + VEL_SIZE + ACC_SIZE)
#define USER_SYNC_DATA_SIZE 5 * sizeof(INT64) + 4 * (POS_SIZE + VEL_SIZE + ACC_SIZE)

struct SPIKEBEACH_API SyncInfo {
	FVector3f Position;
	FVector3f Velocity;
	FVector3f Acceleration;

	TArray<char> Serialize() 
	{
		TArray<char> serialized;
		serialized.Append(reinterpret_cast<char*>(&Position.X), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Position.Y), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Position.Z), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Velocity.X), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Velocity.Y), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Velocity.Z), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Acceleration.X), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Acceleration.Y), sizeof(float));
		serialized.Append(reinterpret_cast<char*>(&Acceleration.Z), sizeof(float));

		return serialized;
	}
	size_t Deserialize(char* buf, size_t len) 
	{
		size_t offset = 0;
		Position.X = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Position.Y = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);		
		Position.Z = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Velocity.X = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);		
		Velocity.Y = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Velocity.Z = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Acceleration.X = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Acceleration.Y = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);
		Acceleration.Z = *reinterpret_cast<float*>(buf + offset);
		offset += sizeof(float);

		return offset;
	}
};

/**
 * 
 */
class SPIKEBEACH_API SyncRes : public Packet
{
public:
	SyncRes() : Packet(PacketId::SYNC_RES) {}
	INT64 syncReqTime;
	TArray<INT64> RTT;
	TArray<SyncInfo> users;

	virtual TArray<char> Serialize() override;
	virtual size_t Deserialize(char* buf, size_t len) override;

	virtual void Process(UWorld* world) override;
};
