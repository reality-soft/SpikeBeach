// Fill out your copyright notice in the Description page of Project Settings.

#include "VolleyballArenaBase.h"
#include "../Ball/Ball.h"
#include "kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
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
	left_player->SetPlayerTurn(EPlayerTurn::PT_SERVICE);
	left_player->SetPlayerRole(EPlayerRole::PR_S_SERVICE);
	
	FVector ServiceSpot;
	float RandomDistance = UKismetMathLibrary::RandomFloatInRange(200, 400);
	switch (service_court)
	{
	case ECourtName::eBeachSideTeam:
		ServiceSpot = left_player->GetActorLocation();
		ServiceSpot.Y += RandomDistance;
		left_player->SetActorLocation(ServiceSpot);
		break;
	case ECourtName::eReefSideTeam:
		ServiceSpot = left_player->GetActorLocation();
		ServiceSpot.Y -= RandomDistance;
		left_player->SetActorLocation(ServiceSpot);
		break;
	}

	auto cast_to_player = (ABasePlayer*)left_player;
	if (cast_to_player)
		//cast_to_player->CanControlBallCursor = true;

	//arena_ball_->Rename(nullptr, left_player->ball_attachment_);
	arena_ball_->AttachToComponent(left_player->ball_attachment_, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
	arena_ball_->SetActorRelativeLocation(FVector(0, 0, 0));
	arena_ball_->PushAndUpdateBallState(EBallState::eAttached);

	// Set Another Player to Proper Turn
	auto my_team_right_player = service_team->GetRightSidePlayer();
	if (my_team_right_player) {
		service_team->GetRightSidePlayer()->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
		service_team->GetRightSidePlayer()->SetPlayerRole(EPlayerRole::PR_S_SERVICE_WAIT);
	}

	auto court = service_court == ECourtName::eReefSideTeam ? ECourtName::eBeachSideTeam : ECourtName::eReefSideTeam;
	auto enemy_team = game_playing_->GetCourtTeam(court);

	auto enemy_team_right_player = enemy_team->GetRightSidePlayer();
	if (enemy_team_right_player) {
		enemy_team_right_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
		enemy_team_right_player->SetPlayerRole(EPlayerRole::PR_S_SERVICE_WAIT);
	}

	auto enemy_team_left_player = enemy_team->GetLeftSidePlayer();
	if (enemy_team_left_player) {
		enemy_team_left_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
		enemy_team_left_player->SetPlayerRole(EPlayerRole::PR_S_SERVICE_WAIT);
	}
}

ECourtName AVolleyballArenaBase::GetPlayerTeam(ABaseCharacter* player)
{
	game_playing_->GetCourtTeam(ECourtName::eReefSideTeam);
	game_playing_->GetCourtTeam(ECourtName::eReefSideTeam);
	return ECourtName();
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
		ball_trigger_->SetWorldLocation(FVector(0, 0, 0));
		arena_ball_->current_predict_.b_hit_land = false;
		return;
	}
	if (arena_ball_->current_predict_.b_hit_land)
	{
		ball_trigger_->SetWorldLocation(arena_ball_->current_predict_.destination);
		ball_trigger_->SetActive(true);
		ball_trigger_->SetVisibility(true);
		ball_trigger_->SetHiddenInGame(false);
		ball_trigger_->SetSphereRadius(200.0f);
		return;
	}
}

void AVolleyballArenaBase::SetPlayerRole()
{
	for (auto& cur_player : Players) {
		switch (cur_player->GetPlayerRole()) {
		case EPlayerRole::PR_D_RECEIVE:
			cur_player->SetPlayerRole(EPlayerRole::PR_A_MOVE_TO_ATTACK_POS);
			cur_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
			break;
		case EPlayerRole::PR_A_MOVE_TO_TOSS_POS:
			cur_player->SetPlayerRole(EPlayerRole::PR_A_TOSS);
			cur_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
			break;
		case EPlayerRole::PR_A_TOSS:
			cur_player->SetPlayerRole(EPlayerRole::PR_A_MOVE_TO_DEFENCE_POS);
			cur_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
			break;
		case EPlayerRole::PR_A_MOVE_TO_ATTACK_POS:
			cur_player->SetPlayerRole(EPlayerRole::PR_A_ATTACK);
			cur_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
			break;
		}
	}
}

void AVolleyballArenaBase::SetPlayerRoleOverTurn()
{
	AVolleyBallTeam* AttackTeam, * DefenceTeam;
	if (arena_ball_->end_pos_[1] >= 810.0f) {
		AttackTeam = game_playing_->GetCourtTeam(ECourtName::eBeachSideTeam);
		DefenceTeam = game_playing_->GetCourtTeam(ECourtName::eReefSideTeam);
	}
	else {
		DefenceTeam = game_playing_->GetCourtTeam(ECourtName::eBeachSideTeam);
		AttackTeam = game_playing_->GetCourtTeam(ECourtName::eReefSideTeam);
	}
	
	auto attack_team_r_player = AttackTeam->GetRightSidePlayer();
	auto attack_team_l_player = AttackTeam->GetLeftSidePlayer();

	float dist_from_ball_to_r = FVector::Distance(attack_team_r_player->GetActorLocation(), arena_ball_->end_pos_);
	float dist_from_ball_to_l = FVector::Distance(attack_team_l_player->GetActorLocation(), arena_ball_->end_pos_);

	if (dist_from_ball_to_r > dist_from_ball_to_l) {
		attack_team_l_player->SetPlayerRole(EPlayerRole::PR_D_RECEIVE);
		attack_team_l_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
		attack_team_r_player->SetPlayerRole(EPlayerRole::PR_A_MOVE_TO_TOSS_POS);
		attack_team_l_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
	}
	else {
		attack_team_l_player->SetPlayerRole(EPlayerRole::PR_A_MOVE_TO_TOSS_POS);
		attack_team_l_player->SetPlayerTurn(EPlayerTurn::PT_OFFENCE);
		attack_team_r_player->SetPlayerRole(EPlayerRole::PR_D_RECEIVE);
		attack_team_l_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
	}

	auto defence_team_r_player = DefenceTeam->GetRightSidePlayer();
	auto defence_team_l_player = DefenceTeam->GetLeftSidePlayer();

	float dist_from_net_to_r = fabs(attack_team_r_player->GetActorLocation().Y - 810.0f);
	float dist_from_net_to_l = fabs(attack_team_l_player->GetActorLocation().Y - 810.0f);
	if (dist_from_net_to_r > dist_from_net_to_l) {
		defence_team_l_player->SetPlayerRole(EPlayerRole::PR_D_FRONT_BEFORE_TOSS);
		defence_team_l_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
		defence_team_r_player->SetPlayerRole(EPlayerRole::PR_D_BACK);
		defence_team_r_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
	}
	else {
		defence_team_r_player->SetPlayerRole(EPlayerRole::PR_D_FRONT_BEFORE_TOSS);
		defence_team_r_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
		defence_team_l_player->SetPlayerRole(EPlayerRole::PR_D_BACK);
		defence_team_l_player->SetPlayerTurn(EPlayerTurn::PT_DEFENCE);
	}
}
