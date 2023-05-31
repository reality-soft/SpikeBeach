// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "PlayerStateEffectSystem.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPIKEBEACH_API UPlayerStateEffectSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateEffectSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Effect System")
		bool InitOwnerInstance(ABasePlayer* player);
	
	UFUNCTION(BlueprintCallable, Category = "Ping")
		bool Spawn_PassPing(FVector location);

	UFUNCTION(BlueprintCallable, Category = "Ping")
		bool Destroy_PassPing();

	UFUNCTION(BlueprintCallable, Category = "Timing Arm")
		void Spawn_TimingArm();

	UFUNCTION(BlueprintCallable, Category = "Timing Arm")
		void Destroy_TimingArm();

	UFUNCTION(BlueprintCallable, Category = "Timing Arm")
		void SetSpawnRate_TimingArm(float spawn_rate);

	UFUNCTION(BlueprintCallable, Category = "Timing Arm")
		void SetColorByTiming_TimingArm(float timing);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
		class ABasePlayer* target_player_ = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Timing Arm")
		UNiagaraComponent* ngcomp_timing_arm_ = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Ping")
		UNiagaraComponent* ngcomp_pass_ping_ = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing Arm")
		FLinearColor subcolor_proper_ {1, 1, 1, 1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing Arm")
		FLinearColor maincolor_proper_ {1, 1, 1, 1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing Arm")
		FLinearColor subcolor_improper_ {1, 1, 1, 1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing Arm")
		FLinearColor maincolor_improper_ {1, 1, 1, 1};


};
