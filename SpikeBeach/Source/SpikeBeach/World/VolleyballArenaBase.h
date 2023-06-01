// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Player/BasePlayer.h"
#include "VolleyballArenaBase.generated.h"

UCLASS()
class SPIKEBEACH_API AVolleyballArenaBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "BallSystem")
		class ABall* arena_ball_ = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "VolleyBallGame")
		class UVolleyBallGame* game_playing_;

	UPROPERTY(BlueprintReadWrite, Category = "Players")
		TArray<ABaseCharacter*> Players;

#pragma region ActionTriggers
	UPROPERTY(BlueprintReadWrite, Category = Components)
		class UBoxComponent* BoxPlayableArea;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* receive_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* dig_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* spike_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* floating_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* toss_trigger_;

	UPROPERTY(BlueprintReadWrite, Category = ActionTriggers)
		class UCapsuleComponent* pass_trigger_;
#pragma endregion

public:
	UFUNCTION(BlueprintCallable, Category = "BallSystem")
		void SetServiceTeam(ECourtName service_team);

	UFUNCTION(BlueprintCallable, Category = "BallSystem")
		ECourtName GetPlayerTeam(ABaseCharacter* player);

	UFUNCTION(BlueprintCallable, Category = "BallSystem")
		void ResetActionTrigger();

public:
	UFUNCTION(BlueprintCallable, Category = "Players")
		void SetPlayerRole();

	UFUNCTION(BlueprintCallable, Category = "Players")
		void SetPlayerRoleOverTurn();

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
