// Fill out your copyright notice in the Description page of Project Settings.


#include "VolleyballArenaBase.h"

bool AVolleyballArenaBase::SetPlayerToTeam(ABasePlayer* player_to_add, ETeamName team_to_add, EPlayerPosition player_position)
{
	if (!player_to_add)
		return false;

	if (team_to_add == ETeamName::eReefSideTeam && player_position == EPlayerPosition::eLeftSidePlayer)
	{		
		if (reef_side_team_.left_side_player == nullptr)
			reef_side_team_.left_side_player = player_to_add;
		else
			UE_LOG(LogTemp, Log, TEXT("Add Player Failed : Already player exist!")); return false;
	}

	if (team_to_add == ETeamName::eReefSideTeam && player_position == EPlayerPosition::eRightSidePlayer)
	{
		if (reef_side_team_.right_side_player == nullptr)
			reef_side_team_.right_side_player = player_to_add;
		else
			UE_LOG(LogTemp, Log, TEXT("Add Player Failed : Already player exist!")); return false;
	}

	if (team_to_add == ETeamName::eBeachSideTeam && player_position == EPlayerPosition::eLeftSidePlayer)
	{
		if (beach_side_team_.left_side_player == nullptr)
			beach_side_team_.left_side_player = player_to_add;
		else
			UE_LOG(LogTemp, Log, TEXT("Add Player Failed : Already player exist!")); return false;
	}

	if (team_to_add == ETeamName::eBeachSideTeam && player_position == EPlayerPosition::eRightSidePlayer)
	{
		if (beach_side_team_.right_side_player == nullptr)
			beach_side_team_.right_side_player = player_to_add;
		else
			UE_LOG(LogTemp, Log, TEXT("Add Player Failed : Already player exist!")); return false;
	}

	
	return true;
}

// Sets default values
AVolleyballArenaBase::AVolleyballArenaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	reef_side_team_.team_name = ETeamName::eReefSideTeam;
	beach_side_team_.team_name = ETeamName::eBeachSideTeam;
}

// Called when the game starts or when spawned
void AVolleyballArenaBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVolleyballArenaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
