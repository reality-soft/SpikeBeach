// Fill out your copyright notice in the Description page of Project Settings.


#include "SyncRes.h"
#include "Kismet/GameplayStatics.h"
#include "../../MultiplayBasePlayer.h"

TArray<char> SyncRes::Serialize()
{
	packetLength = PACKET_SIZE + USER_SYNC_DATA_SIZE;
	TArray<char> serializeVec = Packet::Serialize();
	serializeVec.Reserve(packetLength);

	serializeVec.Append(reinterpret_cast<char*>(&syncReqTime), sizeof(syncReqTime));

	for (size_t i = 0; i < 4; i++)
	{
		serializeVec.Append(reinterpret_cast<char*>(&users[i]), sizeof(users[i]));
	}
	

	for (size_t i = 0; i < 4; i++)
	{
		serializeVec.Append(users[i].Serialize());
	}

	return serializeVec;
}

size_t SyncRes::Deserialize(char* buf, size_t len)
{
	size_t offset = Packet::Deserialize(buf, len);
	users.Empty();
	users.SetNum(4);
	RTT.Empty();
	RTT.SetNum(4);

	syncReqTime = *reinterpret_cast<INT64*>(buf + offset);
	offset += sizeof(syncReqTime);

	for (size_t i = 0; i < RTT.Num(); i++)
	{
		RTT[i] = *reinterpret_cast<INT64*>(buf + offset);
		offset += sizeof(RTT[i]);
	}

	for (size_t i = 0; i < users.Num(); i++)
	{
		offset += users[i].Deserialize(buf + offset, len - offset);
	}

	return offset;
}

void SyncRes::Process(UWorld* world)
{
	AMultiplayBasePlayer* character = Cast<AMultiplayBasePlayer>(UGameplayStatics::GetPlayerCharacter(world, 0));
	character->GetVelocity();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Client Velocity: " + character->GetVelocity().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Server Velocity: " + users[0].Velocity.ToString());

	UE_LOG(LogTemp, Display, TEXT("Client Velocity: %s"), *character->GetVelocity().ToString());
	UE_LOG(LogTemp, Display, TEXT("Server Velocity: %s"), *users[0].Velocity.ToString());

	//FString MyFString;
	//MyFString = MyFString.Printf(TEXT("%lld"), syncReqTime);
	//UE_LOG(LogTemp, Display, TEXT("syncReqTime: %s"), *MyFString);

	//UE_LOG(LogTemp, Display, TEXT("user0"));
	//UE_LOG(LogTemp, Display, TEXT("RTT: %s"), *FString::Printf(TEXT("%lld"), RTT[0]));
	//UE_LOG(LogTemp, Display, TEXT("position: %s"), *users[0].Position.ToString());
	//UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *users[0].Velocity.ToString());
	//UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *users[0].Acceleration.ToString());

	//UE_LOG(LogTemp, Display, TEXT("user1"));
	//UE_LOG(LogTemp, Display, TEXT("RTT: %s"), *FString::Printf(TEXT("%lld"), RTT[1]));
	//UE_LOG(LogTemp, Display, TEXT("position: %s"), *users[1].Position.ToString());
	//UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *users[1].Velocity.ToString());
	//UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *users[1].Acceleration.ToString());

	//UE_LOG(LogTemp, Display, TEXT("user2"));
	//UE_LOG(LogTemp, Display, TEXT("RTT: %s"), *FString::Printf(TEXT("%lld"), RTT[2]));
	//UE_LOG(LogTemp, Display, TEXT("position: %s"), *users[2].Position.ToString());
	//UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *users[2].Velocity.ToString());
	//UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *users[2].Acceleration.ToString());

	//UE_LOG(LogTemp, Display, TEXT("user3"));
	//UE_LOG(LogTemp, Display, TEXT("RTT: %s"), *FString::Printf(TEXT("%lld"), RTT[3]));
	//UE_LOG(LogTemp, Display, TEXT("position: %s"), *users[3].Position.ToString());
	//UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *users[3].Velocity.ToString());
	//UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *users[3].Acceleration.ToString());
}
