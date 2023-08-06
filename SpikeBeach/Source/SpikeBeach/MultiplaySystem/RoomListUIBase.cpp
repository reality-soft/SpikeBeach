// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomListUIBase.h"
#include "Kismet/GameplayStatics.h"
#include "../SpikeBeachGameInstance.h"

void URoomListUIBase::RoomListRequest(FString userAssignedId, FString token, FString clientVersion)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &URoomListUIBase::OnRoomListResponseRecevied);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("userAssignedId", userAssignedId);
	RequestObj->SetStringField("token", token);
	RequestObj->SetStringField("clientVersion", clientVersion);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->SetURL("http://52.197.242.93/Room/Listup");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	UE_LOG(LogTemp, Display, TEXT("Request : %s"), *RequestBody);

	Request->ProcessRequest();
}

void URoomListUIBase::RoomEnterRequest(const FString& roomName, int roomId)
{
	USpikeBeachGameInstance* game_instance =  Cast<USpikeBeachGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	game_instance->ConnectWebSocket();
	game_instance->RoomEnterRequest(roomName, roomId);
}

void URoomListUIBase::OnRoomListResponseRecevied(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Login Response : %s"), *Response->GetContentAsString());

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	if (FCString::Atoi(*ResponseObj->GetStringField("errorCode")) == static_cast<int>(EErrorCode::None)) {
		const TArray<TSharedPtr<FJsonValue>>* RoomList;
		ResponseObj->TryGetArrayField("roomList", RoomList);
		for (const auto& cur_room : *RoomList) {
			FString cur_room_str = cur_room.Get()->AsString();
			FString temp_room_info;

			FRoomInfo cur_room_info;

			cur_room_str.Split(FString("\t"), &temp_room_info, &cur_room_str);
			cur_room_str.Split(FString("\t"), &temp_room_info, &cur_room_str);
			cur_room_info.room_number_ = FCString::Atoi(*temp_room_info);
			cur_room_str.Split(FString("\t"), &temp_room_info, &cur_room_str);
			cur_room_info.room_name_ = temp_room_info;
			cur_room_info.num_people_ = FCString::Atoi(*cur_room_str);

			room_info_.Add(cur_room_info);
		}
	}

	PrintRoomList();
}
