// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginSystemUIBase.h"
#include "RoomListUIBase.generated.h"

USTRUCT(BlueprintType)
struct FRoomInfo 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomInfo")
		int room_number_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomInfo")
		FString room_name_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomInfo")
		int num_people_;
};

/**
 *
 */
UCLASS()
class SPIKEBEACH_API URoomListUIBase : public ULoginSystemUIBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomInfo")
		TArray<FRoomInfo> room_info_;

public:
	UFUNCTION(BlueprintCallable)
		void RoomListRequest(FString userAssignedId, FString token, FString clientVersion);

	UFUNCTION(BlueprintCallable)
		void RoomEnterRequest(const FString& roomName, int roomId);

	void OnRoomListResponseRecevied(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintImplementableEvent)
		void PrintRoomList();

};
