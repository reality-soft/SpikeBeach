// Fill out your copyright notice in the Description page of Project Settings.

#include "VolleyballArenaBase.h"
#include "Components/SphereComponent.h"
#include "VolleyBallTeam.h"
#include "VolleyBallGame.h"

void AVolleyballArenaBase::SetServiceTeam(ECourtName service_court)
{
	game_playing_->SetServiceCourt(service_court);

	auto service_team = game_playing_->GetCourtTeam(service_court);
	
	auto left_player = service_team->GetLeftSidePlayer();
	
	if (!left_player)
		return;
	
	left_player->SetServiceMode("Floating");
	arena_ball_->AttachToComponent(left_player->ball_attachment_, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
	arena_ball_->PushAndUpdateBallState(EBallState::eAttached);

	// Set Another Player to Proper Turn
	auto my_team_right_player = service_team->GetRightSidePlayer();
	if(my_team_right_player)
		service_team->GetRightSidePlayer()->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
	auto court = service_court == ECourtName::eReefSideTeam ? ECourtName::eBeachSideTeam : ECourtName::eReefSideTeam;
	auto enemy_team = game_playing_->GetCourtTeam(court);

	auto enemy_team_right_player = enemy_team->GetRightSidePlayer();
	if (enemy_team_right_player)
		enemy_team_right_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);

	auto enemy_team_left_player = enemy_team->GetLeftSidePlayer();
	if (enemy_team_left_player)
		enemy_team_left_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);

}

// Sets default values
AVolleyballArenaBase::AVolleyballArenaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	game_playing_ = CreateDefaultSubobject<UVolleyBallGame>(TEXT("VolleyBallGameComponent"));
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
