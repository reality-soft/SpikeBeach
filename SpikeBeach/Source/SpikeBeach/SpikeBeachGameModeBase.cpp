// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeBeachGameModeBase.h"
#include "SpikeBeachGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ASpikeBeachGameModeBase::BeginPlay()
{
	USpikeBeachGameInstance* game_instance = Cast<USpikeBeachGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	game_instance->InitSocket();
}
