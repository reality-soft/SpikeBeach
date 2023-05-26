// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/BasePlayer.h"
#include "OffsetTestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SPIKEBEACH_API AOffsetTestPlayer : public ABasePlayer
{
	GENERATED_BODY()
	
protected:
	/* Called for LClick Input*/
		void LClickTriggered(const FInputActionValue& Value) override {};
	/* Called for LClick Input*/
		void LClickCompleted(const FInputActionValue& Value) override {};
	/* Called for RClick Input*/
		void RClickTriggered(const FInputActionValue& Value) override {};
	/* Called for RClick Input*/
		void RClickCompleted(const FInputActionValue& Value) override {};
protected:
	virtual void ServiceHitBall() override;

	// DEFENCE
	virtual void ReceiveBall() override;
	virtual void DigBall() override;
	virtual void BlockBall() override;

	// OFFENCE
	virtual void TossBall() override;
	virtual void PassBall() override;
	virtual void SpikeBall() override;
	virtual void FloatingBall() override;
protected:
	virtual void PlayServiceAnimation() override;
	virtual void PlayPassAnimation() override;
	virtual void PlayAttackAnimation() override;
	virtual void PlayReceiveAnimation() override;
	virtual void PlayBlockAnimation() override;
};
