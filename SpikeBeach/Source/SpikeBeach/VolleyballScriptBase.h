// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/TriggerBox.h"
#include "VolleyballScriptBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API AVolleyballScriptBase : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule")
		ATriggerBox* t1_area_ = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule")
		ATriggerBox* t2_area_ = nullptr;

	
	UFUNCTION(BlueprintCallable, Category = "GameRule")
		void SetRule();
};
