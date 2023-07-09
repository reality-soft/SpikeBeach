// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterUIBase.h"

void URegisterUIBase::RegisterRequest(FString userAssignedId, FString nickname, FString password)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &URegisterUIBase::OnRegisterResponseReceived);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("userAssignedId", userAssignedId);
	RequestObj->SetStringField("nickname", nickname);
	RequestObj->SetStringField("password", password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->SetURL("http://52.197.242.93/Register");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	UE_LOG(LogTemp, Display, TEXT("Request : %s"), *RequestBody);

	Request->ProcessRequest();
}

void URegisterUIBase::OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Register Response : %s"), *Response->GetContentAsString());
	
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	if (FCString::Atoi(*ResponseObj->GetStringField("errorCode")) == static_cast<int>(EErrorCode::None)) {
		OnSuccess();
	}
}
