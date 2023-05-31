// Fill out your copyright notice in the Description page of Project Settings.

#include "VolleyBallTeam.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AVolleyBallTeam::AVolleyBallTeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AVolleyBallTeam::AddPlayerToTeam(ABaseCharacter* player) {
	if (left_side_player == nullptr)
	{
		left_side_player = player;
		left_side_player->SetMyTeam(this);
		return true;
	}

	if (right_side_player == nullptr)
	{
		right_side_player = player;
		right_side_player->SetMyTeam(this);
		return true;
	}

	return false;
}

void AVolleyBallTeam::WinScore() {
	score++;
}

void AVolleyBallTeam::WinSet() {
	set++;
}

void AVolleyBallTeam::SwapPlayerPos() {
	ABaseCharacter* temp = right_side_player;
	right_side_player = left_side_player;
	left_side_player = temp;
}

void AVolleyBallTeam::UpdateBallCursor(FVector2D cursor)
{
	if (team_box_ == nullptr ||
		ball_cursor_capsule_ == nullptr ||
		ngsystem_ball_cursor_ == nullptr)
		return;

	if (ngcomp_ball_cursor_ == nullptr)
	{
		ngcomp_ball_cursor_ = UNiagaraFunctionLibrary::SpawnSystemAttached(ngsystem_ball_cursor_, ball_cursor_capsule_, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false);
	}

	FVector box_extend;
	FVector cursor_3D_pos;

	box_extend = team_box_->GetScaledBoxExtent();
	cursor_3D_pos = team_box_->GetComponentLocation();

	switch (court)
	{
	case ECourtName::eReefSideTeam:
		box_extend.Y *= -1.0f;
		break;
	case ECourtName::eBeachSideTeam:
		box_extend.X *= -1.0f;
		break;
	}

	float X_local = box_extend.X * cursor.X;
	float Y_local = box_extend.Y * cursor.Y;

	FVector local_3D_pos(X_local, Y_local, 100.0f);

	ball_cursor_capsule_->SetWorldLocation(cursor_3D_pos + local_3D_pos);
	ball_cursor_capsule_->SetVisibility(true);
	ball_cursor_capsule_->SetHiddenInGame(false);
}

void AVolleyBallTeam::ClearBallCursor()
{
	if (ngcomp_ball_cursor_)
	{
		ngcomp_ball_cursor_->DestroyComponent();
		ngcomp_ball_cursor_ = nullptr;
	}
}

bool AVolleyBallTeam::IsVectorInTeamBox(FVector vector)
{
	auto box = team_box_;

	FVector box_extent = box->GetScaledBoxExtent();
	FVector box_pos = box->GetComponentLocation();

	float min_x = box_pos.X - box_extent.X;
	float max_x = box_pos.X + box_extent.X;

	float min_y = box_pos.Y - box_extent.Y;
	float max_y = box_pos.Y + box_extent.Y;

	if (max_x > vector.X && vector.X > min_x)
	{
		if (max_y > vector.Y && vector.Y > min_y)
		{
			return true;
		}
	}
	return false;
}

// Called when the game starts or when spawned
void AVolleyBallTeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVolleyBallTeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

