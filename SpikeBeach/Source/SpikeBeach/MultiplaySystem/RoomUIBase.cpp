// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomUIBase.h"
#include "Kismet/GameplayStatics.h"
#include "../SpikeBeachGameInstance.h"

void URoomUIBase::ExitRoom()
{
	USpikeBeachGameInstance* game_instance = Cast<USpikeBeachGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	game_instance->SendRoomLeaveRequest();
}
