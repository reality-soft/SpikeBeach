// Fill out your copyright notice in the Description page of Project Settings.

#include "VolleyBallTeam.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AVolleyBallTeam::AVolleyBallTeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVolleyBallTeam::UpdateBallCursor(FVector2D cursor)
{
	if (team_box_ == nullptr ||
		ball_cursor_ == nullptr)
		return;

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

	ball_cursor_->SetWorldLocation(cursor_3D_pos + local_3D_pos);
	ball_cursor_->SetVisibility(true);
	ball_cursor_->SetHiddenInGame(false);
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

