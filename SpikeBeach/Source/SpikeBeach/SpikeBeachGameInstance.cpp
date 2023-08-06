// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeBeachGameInstance.h"
#include "WebSocketsModule.h"
#include "MultiplaySystem/WebSocketRequests/RoomEnterRequest.h"

void USpikeBeachGameInstance::ConnectWebSocket()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://52.197.242.93:80/Room/ws/Enter");

	WebSocket->OnConnected().AddLambda([]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");
		});

	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
		});

	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed " + Reason);
		});

	WebSocket->OnMessage().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received message: " + MessageString);
		});


	WebSocket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
		{
			const char* charArray = nullptr;
			if (Data) {
				memcpy(&charArray, Data, Size);
			}
			
			FString s(charArray, Size);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received message: " + s);
		});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
		});

	WebSocket->Connect();
}

void USpikeBeachGameInstance::Shutdown()
{
	Super::Shutdown();

	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	Super::Shutdown();
}

void USpikeBeachGameInstance::RoomEnterRequest(const FString& roomName, int roomId)
{
	URoomEnterRequest room_enter_request;

	room_enter_request.clientVersion = client_version;
	room_enter_request.token = login_token_;
	room_enter_request.userAssignedId = login_id;
	room_enter_request.roomId = roomId;
	
	dataToSend.Empty();
	dataToSend = room_enter_request.Serialize();
	WebSocket->Send(dataToSend.GetData(), dataToSend.Num(), true);
}
