// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperNavMovementComponent.h"

DEFINE_LOG_CATEGORY(LogPaperNavMovementComponent)

void UPaperNavMovementComponent::StopActiveMovement()
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("StopActiveMovement"));
}

void UPaperNavMovementComponent::StopMovementImmediately()
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("StopMovementImmediately"));
}

FBasedPosition UPaperNavMovementComponent::GetActorFeetLocationBased() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("GetActorFeetLocationBased"));
	return Super::GetActorFeetLocationBased();
}

void UPaperNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("RequestDirectMove: %s; %s"), *MoveVelocity.ToString(), bForceMaxSpeed ? TEXT("true") : TEXT("false"));
}

void UPaperNavMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("RequestPathMove: %s"), *MoveInput.ToString());
}

bool UPaperNavMovementComponent::CanStopPathFollowing() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("CanStopPathFollowing"));
	return Super::CanStopPathFollowing();
}

float UPaperNavMovementComponent::GetPathFollowingBrakingDistance(float MaxSpeed) const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("GetPathFollowingBrakingDistance: %f"), MaxSpeed);
	return Super::GetPathFollowingBrakingDistance(MaxSpeed);
}

bool UPaperNavMovementComponent::CanStartPathFollowing() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("CanStartPathFollowing"));
	return Super::CanStartPathFollowing();
}

bool UPaperNavMovementComponent::IsCrouching() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsCrouching"));
	return Super::IsCrouching();
}

bool UPaperNavMovementComponent::IsFalling() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsFalling"));
	return Super::IsFalling();
}

bool UPaperNavMovementComponent::IsMovingOnGround() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsMovingOnGround"));
	return Super::IsMovingOnGround();
}

bool UPaperNavMovementComponent::IsSwimming() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsSwimming"));
	return Super::IsSwimming();
}

bool UPaperNavMovementComponent::IsFlying() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsFlying"));
	return Super::IsFlying();
}
