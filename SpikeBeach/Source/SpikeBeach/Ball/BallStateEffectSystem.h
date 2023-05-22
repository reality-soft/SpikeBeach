// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Ball.h" 

#include "BallStateEffectSystem.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPIKEBEACH_API UBallStateEffectSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallStateEffectSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Effect System")
		bool InitOwnerInstance(ABall* ball);

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void SpawnArcTrail();

	UFUNCTION(BlueprintCallable, Category = "Landing Point")
		void SpawnLandingPoint(FVector world_location);

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void DestroyArcTrail();

	UFUNCTION(BlueprintCallable, Category = "Landing Point")
		void DestroyLandingPoint();

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void SetArcTrailSpawnRate(float spawn_rate);

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void SetTrailColor_Stable();

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void SetTrailColor_Offensive();

	UFUNCTION(BlueprintCallable, Category = "Arc Trail")
		void SetTrailColor_Wrong();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Arc Trail")
		UNiagaraComponent* ngcomp_arc_trail_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Landing Point")
		UNiagaraComponent* ngcomp_landing_point_ = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "EffectSystem")
		ABall* owner_ball_ = nullptr;

	//Trail Color Set
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor subcolor_stable_ = {1, 1, 1, 1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor subcolor_offensive_ = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor subcolor_wrong_ = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor maincolor_stable_ = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor maincolor_offensive_ = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Trail")
		FLinearColor maincolor_wrong_ = { 1, 1, 1, 1 };
};
