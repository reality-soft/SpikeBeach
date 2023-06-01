// Fill out your copyright notice in the Description page of Project Settings.


#include "VolleyBallGame.h"
#include "VolleyBallTeam.h"
#include "GameFramework/CharacterMovementComponent.h"	

// Sets default values for this component's properties
UVolleyBallGame::UVolleyBallGame()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Add 2 Team Array 
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

void UVolleyBallGame::ReturnPlayerToCourtPoint()
{
	auto reef_side_team = GetCourtTeam(ECourtName::eReefSideTeam);
	auto reef_r_player = reef_side_team->GetRightSidePlayer();
	auto reef_l_player = reef_side_team->GetLeftSidePlayer();
	if (reef_r_player && reef_l_player)
	{
		reef_r_player->SetActorLocation(ReefCourtRight.GetLocation());
		reef_r_player->SetActorRotation(ReefCourtRight.GetRotation());
		reef_r_player->GetCharacterMovement()->StopMovementImmediately();
		reef_r_player->is_montage_started_ = false;
		reef_l_player->SetActorLocation(ReefCourtLeft.GetLocation());
		reef_l_player->SetActorRotation(ReefCourtLeft.GetRotation());
		reef_l_player->GetCharacterMovement()->StopMovementImmediately();
		reef_l_player->is_montage_started_ = false;
	}

	auto beach_side_team = GetCourtTeam(ECourtName::eBeachSideTeam);
	auto beach_r_player = beach_side_team->GetRightSidePlayer();
	auto beach_l_player = beach_side_team->GetLeftSidePlayer();
	if (beach_r_player && beach_l_player)
	{
		beach_r_player->SetActorLocation(BeachCourtRight.GetLocation());
		beach_r_player->SetActorRotation(BeachCourtRight.GetRotation());
		beach_r_player->GetCharacterMovement()->StopMovementImmediately();
		beach_r_player->is_montage_started_ = false;
		beach_l_player->SetActorLocation(BeachCourtLeft.GetLocation());
		beach_l_player->SetActorRotation(BeachCourtLeft.GetRotation());
		beach_l_player->GetCharacterMovement()->StopMovementImmediately();
		beach_l_player->is_montage_started_ = false;
	}
}

void UVolleyBallGame::ChangeCourt()
{
	auto reef_side_team = GetCourtTeam(ECourtName::eReefSideTeam);
	auto beach_side_team = GetCourtTeam(ECourtName::eBeachSideTeam);

	TeamPlayingVolleyBall[(int)ECourtName::eBeachSideTeam] = reef_side_team;
	reef_side_team->SetTeamCourt(ECourtName::eBeachSideTeam);

	TeamPlayingVolleyBall[(int)ECourtName::eReefSideTeam] = beach_side_team;
	beach_side_team->SetTeamCourt(ECourtName::eReefSideTeam);
	
	// team box change
	auto temp_box = reef_side_team->team_box_;
	reef_side_team->team_box_ = beach_side_team->team_box_;
	beach_side_team->team_box_ = temp_box;
}
