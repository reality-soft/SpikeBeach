// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BaseCharacter.h"
#include "VolleyBallTeam.generated.h"

UCLASS()
class SPIKEBEACH_API AVolleyBallTeam : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		FString team_name = TEXT("Default Team Name");

	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		ECourtName court;

	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		ABaseCharacter* right_side_player = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		ABaseCharacter* left_side_player = nullptr;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UBoxComponent* team_box_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UCapsuleComponent* ball_cursor_capsule_;

	UPROPERTY(BlueprintReadWrite, Category = "Game Play")
		class UNiagaraSystem* ngsystem_ball_cursor_;

	UPROPERTY(BlueprintReadOnly, Category = "Game Play")
		class UNiagaraComponent* ngcomp_ball_cursor_;

private:
	// Ball Cursor
	bool is_cursor_active_ = false;

	// SCORE
	UINT score;
	UINT set;
  
#pragma region SETTER
public:
	UFUNCTION(BlueprintCallable)
		void SetTeamName(FString name) { team_name = name; }

	UFUNCTION(BlueprintCallable)
		FString GetetTeamName() { return team_name; }

	UFUNCTION(BlueprintCallable)
		void SetTeamCourt(ECourtName court_name) { court = court_name; }
#pragma endregion

#pragma region GETTER
public:
	UFUNCTION(BlueprintCallable)
		ABaseCharacter* GetLeftSidePlayer() { return left_side_player; }

	UFUNCTION(BlueprintCallable)
		ABaseCharacter* GetRightSidePlayer() { return right_side_player; }

	UFUNCTION(BlueprintCallable)
		ECourtName GetCourtName() { return court; }
#pragma endregion

public:
	// Sets default values for this actor's properties
	AVolleyBallTeam();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool AddPlayerToTeam(ABaseCharacter* player);

	UFUNCTION(BlueprintCallable)
		void WinScore();

	UFUNCTION(BlueprintCallable)
		void WinSet();

	UFUNCTION(BlueprintCallable)
		void SwapPlayerPos();

	UFUNCTION(BlueprintCallable, Category = "Game Play")
		void UpdateBallCursor(FVector2D cursor);

	UFUNCTION(BlueprintCallable, Category = "Game Play")
		void ClearBallCursor();

	UFUNCTION(BlueprintCallable)
		bool IsVectorInTeamBox(FVector vector);
};

