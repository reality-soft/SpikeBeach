// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeBeachGameInstance.h"
#include "WebSocketsModule.h"
#include "OpeningGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplaySystem/WebSocketPackets/Requests/RoomEnterRequest.h"
#include "MultiplaySystem/WebSocketPackets/Responses/RoomEnterResponse.h"
#include "MultiplaySystem/WebSocketPackets/Notifies/RoomEnterNotify.h"
#include "MultiplaySystem/WebSocketPackets/Requests/RoomLeaveRequest.h"
#include "MultiplaySystem/WebSocketPackets/Responses/RoomLeaveResponse.h"
#include "MultiplaySystem/WebSocketPackets/Notifies/RoomLeaveNotify.h"
#include "MultiplaySystem/WebSocketPackets/Requests/ReadyRequest.h"
#include "MultiplaySystem/WebSocketPackets/Responses/ReadyResponse.h"
#include "MultiplaySystem/WebSocketPackets/Notifies/ReadyNotify.h"
#include "MultiplaySystem/WebSocketPackets/PacketIdDef.h"
#include "MultiplaySystem/WebSocketPackets/ErrorCode.h"

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


	WebSocket->OnRawMessage().AddLambda([this](const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
		{
			ProcessPacket(Data, Size, BytesRemaining);
		});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
		});

	WebSocket->Connect();
}

void USpikeBeachGameInstance::ProcessPacket(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	int offset = 0;
	const int32* int32Ptr = static_cast<const int32*>(Data);
	const int16* int16Ptr = static_cast<const int16*>(Data);
	PacketIdDef packetIdEnum = static_cast<PacketIdDef>(int32Ptr[0]);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Packet Id: " + FString::FromInt(int32Ptr[0]));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "ErrorCode: " + FString::FromInt(int16Ptr[2]));

	switch (packetIdEnum) {
	case PacketIdDef::RoomEnterRes:
	{
		RoomEnterResponse roomEnterResponse;
		roomEnterResponse.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("RoomInfoString : %s"), *roomEnterResponse.roomInfoString);
		UE_LOG(LogTemp, Display, TEXT("ErrorCode : %s"), *FString::FromInt(roomEnterResponse.errorCode));

		Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->EnterRoom(roomEnterResponse.roomInfoString);

		break;
	}
	case PacketIdDef::RoomEnterNtf:
	{
		RoomEnterNotify roomEnterNotify;
		roomEnterNotify.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("enterUserNick : %s"), *roomEnterNotify.enterUserNick);
		
		Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->AddUserToRoom(roomEnterNotify.enterUserNick);

		break;
	}
	case PacketIdDef::RoomLeaveRes:
	{
		RoomLeaveResponse roomLeaveResponse;
		roomLeaveResponse.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("ErrorCode : %s"), *FString::FromInt(roomLeaveResponse.errorCode));

		Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ExitRoom();

		break;
	}
	case PacketIdDef::RoomLeaveNtf:
	{
		RoomLeaveNotify roomLeaveNotify;
		roomLeaveNotify.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("leaveUserNick : %s"), *roomLeaveNotify.leaveInfoString);

		Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->RemoveUserFromRoom(roomLeaveNotify.leaveInfoString, roomLeaveNotify.leaveInfoString);

		break;
	}
	case PacketIdDef::RoomReadyRes:
	{
		ReadyResponse roomReadyResponse;
		roomReadyResponse.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("ErrorCode : %s"), *FString::FromInt(roomReadyResponse.errorCode));

		Cast<AOpeningGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ExitRoom();

		break;
	}
	case PacketIdDef::RoomReadyNtf:
	{
		ReadyNotify roomReadyNotify;
		roomReadyNotify.Deserialize(static_cast<const uint8*>(Data));

		UE_LOG(LogTemp, Display, TEXT("leaveUserNick : %s"), *roomReadyNotify.teamString);

		break;
	}
	}
}

void USpikeBeachGameInstance::CloseWebSocket()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

void USpikeBeachGameInstance::Shutdown()
{
	Super::Shutdown();
}

void USpikeBeachGameInstance::SendRoomEnterRequest(const FString& roomName, int roomId)
{
	RoomEnterRequest room_enter_request;

	room_enter_request.clientVersion = client_version;
	room_enter_request.token = login_token_;
	room_enter_request.userAssignedId = login_id;
	room_enter_request.roomId = roomId;
	
	dataToSend.Empty();
	dataToSend = room_enter_request.Serialize();
	WebSocket->Send(dataToSend.GetData(), dataToSend.Num(), true);
}

void USpikeBeachGameInstance::SendRoomLeaveRequest()
{
	RoomLeaveRequest room_leave_request;

	dataToSend = room_leave_request.Serialize();
	WebSocket->Send(dataToSend.GetData(), dataToSend.Num(), true);
}

void USpikeBeachGameInstance::SendRoomReadyRequest(int team)
{
	ReadyRequest room_ready_request;
	room_ready_request.team = static_cast<Team>(team);

	dataToSend = room_ready_request.Serialize();
	WebSocket->Send(dataToSend.GetData(), dataToSend.Num(), true);
}
