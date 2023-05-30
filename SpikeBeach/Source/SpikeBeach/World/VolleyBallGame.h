// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../BaseCharacter.h"
#include "VolleyBallGame.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPIKEBEACH_API UVolleyBallGame : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		TArray<class AVolleyBallTeam*> TeamPlayingVolleyBall;

	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		ECourtName service_court;

	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		FVector ReefCourtLeft;
	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		FVector ReefCourtRight;
	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		FVector BeachCourtLeft;
	UPROPERTY(BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
		FVector BeachCourtRight;
public:
	// Sets default values for this component's properties
	UVolleyBallGame();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
#pragma region GETTER
	UFUNCTION(BlueprintCallable)
		class AVolleyBallTeam* GetCourtTeam(ECourtName court) { return TeamPlayingVolleyBall[(int)court]; }

	UFUNCTION(BlueprintCallable)
		ECourtName GetServiceCourt() { return service_court; }
#pragma endregion

#pragma region SETTER
	UFUNCTION(BlueprintCallable)
		void SetCourtTeam(ECourtName court, AVolleyBallTeam* team) { TeamPlayingVolleyBall[(int)court] = team; }
	UFUNCTION(BlueprintCallable)
		void SetServiceCourt(ECourtName court) { service_court = court; }
#pragma endregion

	UFUNCTION(BlueprintCallable)
		void ReturnPlayerToCourtPoint();

	UFUNCTION(BlueprintCallable)
		void ChangeCourt();
};
