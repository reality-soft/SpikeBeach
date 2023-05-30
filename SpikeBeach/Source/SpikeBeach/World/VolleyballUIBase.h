// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VolleyballUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API UVolleyballUIBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Team UI")
		class AVolleyballArenaBase* arena_;

};
