// Fill out your copyright notice in the Description page of Project Settings.


#include "OffsetTestPlayer.h"

void AOffsetTestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOffsetTestPlayer::ServiceHitBall()
{
	auto SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("%s Service Offset (%f, %f, %f)"), ServiceMode, GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::ReceiveBall()
{
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightLowerArm")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Receive %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::DigBall()
{
	auto SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Dig %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::BlockBall()
{
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightLowerArm")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Block %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);

}

void AOffsetTestPlayer::TossBall()
{
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftHand")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Toss %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);

}

void AOffsetTestPlayer::PassBall()
{
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightLowerArm")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Pass %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::SpikeBall()
{
	auto SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Spike %s, Offset (%f, %f, %f)"), *SpikeMode.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::FloatingBall()
{
	auto LeftSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("LeftLowerArm")));
	auto RightSocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightLowerArm")));
	auto SocketLocation = (LeftSocketLocation + RightSocketLocation) / 2;
	auto ActorLocation = GetActorLocation();
	auto Offset = SocketLocation - ActorLocation;

	auto quat = FQuat::FindBetweenVectors(GetActorForwardVector(), FVector(1.0f, 0.0f, 0.0f));
	auto GeneralOffset = quat.RotateVector(Offset);

	UE_LOG(LogTemp, Log, TEXT("Floating %s, Offset (%f, %f, %f)"), *Direction.ToString(), GeneralOffset.X, GeneralOffset.Y, GeneralOffset.Z);
}

void AOffsetTestPlayer::PlayServiceAnimation()
{
	float PlayRate = 1.0f;

	PlayAnimMontage(ServiceMontage, PlayRate, ServiceMode);
}

void AOffsetTestPlayer::PlayPassAnimation()
{
	float PlayRate = 1.0f;

	switch (OffenceMode)
	{
	case EOffenceMode::OM_TOSS:
		PlayAnimMontage(TossMontage, PlayRate, Direction);
		break;
	case EOffenceMode::OM_PASS:
		PlayAnimMontage(PassMontage, PlayRate, Direction);
		break;
	}
}

void AOffsetTestPlayer::PlayAttackAnimation()
{
	float PlayRate = 1.0f;
	switch (OffenceMode)
	{
	case EOffenceMode::OM_SPIKE:
		PlayAnimMontage(SpikeMontage, PlayRate, SpikeMode);
		break;
	case EOffenceMode::OM_FLOATING:
		PlayAnimMontage(FloatingMontage, PlayRate, Direction);
		break;
	}
}

void AOffsetTestPlayer::PlayReceiveAnimation()
{
	float PlayRate = 1.0f;
	switch (DefenceMode)
	{
	case EDefenceMode::DM_DIG:
		PlayAnimMontage(DigMontage, PlayRate, Direction);
		break;
	case EDefenceMode::DM_RECEIVE:
		PlayAnimMontage(ReceiveMontage, PlayRate, Direction);
		break;
	}
}

void AOffsetTestPlayer::PlayBlockAnimation()
{
	PlayAnimMontage(BlockMontage, 1.0f, Direction);
}