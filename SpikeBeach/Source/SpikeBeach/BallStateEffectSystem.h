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
	
	UFUNCTION(BlueprintCallable, Category = "EffectSystem")
		bool InitOwnerInstance(ABall* ball);

public:

	UPROPERTY(BlueprintReadWrite, Category = "EffectSystem")
		ABall* owner_ball_ = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "EffectSystem")
		UNiagaraSystem* arc_trail_fx_;

	UPROPERTY(BlueprintReadWrite, Category = "EffectSystem")
		UNiagaraSystem* landing_point_fx_;

	UFUNCTION(BlueprintCallable, Category = "EffectSystem")
		void ShowArcTrail();
};
