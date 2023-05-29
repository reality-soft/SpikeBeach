// Fill out your copyright notice in the Description page of Project Settings.

#include "VolleyballArenaBase.h"
#include "kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

bool AVolleyballArenaBase::SetPlayerToTeam(ABaseCharacter* player_to_add, ETeamName team_to_add, EPlayerPosition player_position)
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

void AVolleyballArenaBase::SetServiceTeam(ETeamName service_team, bool start_new_set)
{
	service_team_ = service_team;

	if (start_new_set == false)
		return;

	switch (service_team)
	{
	case ETeamName::eReefSideTeam:
		if (reef_side_team_.left_side_player)
		{
			arena_ball_->AttachToComponent(reef_side_team_.left_side_player->ball_attachment_, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
			arena_ball_->PushAndUpdateBallState(EBallState::eAttached);
		}
		break;

	case ETeamName::eBeachSideTeam:
		if (beach_side_team_.left_side_player)
		{
			arena_ball_->AttachToComponent(beach_side_team_.left_side_player->ball_attachment_, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
			arena_ball_->PushAndUpdateBallState(EBallState::eAttached);
		}
		break;
	}
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

	UpdateBallTrigger();

}

void AVolleyballArenaBase::UpdateBallTrigger()
{
	if (ball_trigger_ == nullptr)
		return;

	if (arena_ball_->current_ball_state_ == EBallState::eDropped)
	{
		ball_trigger_->SetActive(false);
		return;
	}
	if (arena_ball_->current_predict_.b_hit_land)
	{
		ball_trigger_->SetWorldLocation(arena_ball_->current_predict_.destination);
		ball_trigger_->SetActive(true);
		ball_trigger_->SetVisibility(true);
		ball_trigger_->SetHiddenInGame(false);
		ball_trigger_->SetSphereRadius(100.0f);
		return;
	}
}

void AVolleyballArenaBase::UpdateBallCursor(ETeamName agaist_team, FVector2D cursor)
{
	if (reef_team_box_ == nullptr ||
		beach_team_box_ == nullptr)
		return;

	FVector box_extend;
	FVector cursor_3D_pos;

	switch (agaist_team)
	{
	case ETeamName::eReefSideTeam:
		box_extend = reef_team_box_->GetScaledBoxExtent();
		box_extend.Y *= -1.0f;
		cursor_3D_pos = reef_team_box_->GetComponentLocation();
		break;
	case ETeamName::eBeachSideTeam:
		box_extend = beach_team_box_->GetScaledBoxExtent();
		box_extend.X *= -1.0f;
		cursor_3D_pos = beach_team_box_->GetComponentLocation();
		break;
	}

	float X_local = box_extend.X * cursor.X;
	float Y_local = box_extend.Y * cursor.Y;

	FVector local_3D_pos(X_local, Y_local, 100.0f);

	ball_cursor_->SetWorldLocation(cursor_3D_pos + local_3D_pos);
	ball_cursor_->SetVisibility(true);
	ball_cursor_->SetHiddenInGame(false);
}
