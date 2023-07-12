// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomListUIBase.h"

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

void URoomListUIBase::OnRoomListResponseRecevied(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Login Response : %s"), *Response->GetContentAsString());

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	if (FCString::Atoi(*ResponseObj->GetStringField("errorCode")) == static_cast<int>(EErrorCode::None)) {

	}
}
