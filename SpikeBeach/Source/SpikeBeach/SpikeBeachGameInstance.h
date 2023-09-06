// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "MultiplaySystem/ClientSocket.h"
#include "SpikeBeachGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API USpikeBeachGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginInfo")
		FString login_id = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginInfo")
		FString login_token_ = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginInfo")
		FString client_version = "v1.0.0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginInfo")
		int room_number = -1;

	FString ipAddress;
	uint16 portNum = 11021;

	AClientSocket* clientSocket;

	TSharedPtr<IWebSocket> WebSocket;
	TArray<uint8_t> dataToSend;

	UFUNCTION(BlueprintCallable)
		void SendRoomEnterRequest(const FString& roomName, int roomId);
	
	UFUNCTION(BlueprintCallable)
		void SendRoomLeaveRequest();

	UFUNCTION(BlueprintCallable)
		void SendRoomReadyRequest(int team);

	UFUNCTION(BlueprintCallable)
		void SendRoomUnreadyRequest();

	UFUNCTION(BlueprintCallable)
		void SendGameStartRequest();

	UFUNCTION(BlueprintCallable)
		virtual void ConnectWebSocket();

	void ProcessPacket(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);

	UFUNCTION(BlueprintCallable)
		void InitSocket();

	UFUNCTION(BlueprintCallable)
		void CloseWebSocket();

	INT64 getCurDelayTime() {
		return clientSocket->getCurDelayTime();
	}
};
