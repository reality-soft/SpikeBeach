// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Player/BasePlayer.h"
#include "../Ball/Ball.h"

#include "VolleyballArenaBase.generated.h"

UENUM(BlueprintType)
enum class ETeamName : uint8
{
	eNone,
	eReefSideTeam,
	eBeachSideTeam
};
UENUM(BlueprintType)
enum class EPlayerPosition : uint8
{
	eNone,
	eRightSidePlayer,
	eLeftSidePlayer
};

USTRUCT(BlueprintType)
struct FVolleyballTeam
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Team")
		ETeamName team_name = ETeamName::eNone;

	UPROPERTY(BlueprintReadWrite, Category = "Team")
		ABaseCharacter* right_side_player = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Team")
		ABaseCharacter* left_side_player = nullptr;
};

UCLASS()
class SPIKEBEACH_API AVolleyballArenaBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Team")
		FVolleyballTeam reef_side_team_;

	UPROPERTY(BlueprintReadWrite, Category = "Team")
		FVolleyballTeam beach_side_team_;

	UPROPERTY(BlueprintReadWrite, Category = "BallSystem")
		ABall* arena_ball_ = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class USphereComponent* ball_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UCapsuleComponent* ball_cursor_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UBoxComponent* reef_team_box_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UBoxComponent* beach_team_box_;

	UPROPERTY(BlueprintReadOnly, Category = "Game Play")
		ETeamName service_team_ = ETeamName::eNone;

public:
	UFUNCTION(BlueprintCallable, Category = "Team")
		bool SetPlayerToTeam(ABaseCharacter* player_to_add, ETeamName team_to_add, EPlayerPosition player_position);

	UFUNCTION(BlueprintCallable, Category = "Team")
		void SetServiceTeam(ETeamName service_team, bool start_new_set);

	UFUNCTION(BlueprintCallable, Category = "Game Play")
		void UpdateBallTrigger();

	UFUNCTION(BlueprintCallable, Category = "Game Play")
		void UpdateBallCursor(ETeamName agaist_team, FVector2D cursor);

public:
	// Sets default values for this actor's properties
	AVolleyballArenaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
