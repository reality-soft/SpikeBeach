// Fill out your copyright notice in the Description page of Project Settings.


#include "VolleyBallGame.h"

// Sets default values for this component's properties
UVolleyBallGame::UVolleyBallGame()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	TeamPlayingVolleyBall.Add(nullptr);
	TeamPlayingVolleyBall.Add(nullptr);
}


// Called when the game starts
void UVolleyBallGame::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVolleyBallGame::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

