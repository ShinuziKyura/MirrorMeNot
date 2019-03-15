// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperNavMovementComponent.h"
#include "Components/PrimitiveComponent.h"

DEFINE_LOG_CATEGORY(LogPaperNavMovementComponent)

UPaperNavMovementComponent::UPaperNavMovementComponent(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, InputVector(FVector2D::ZeroVector)
{
	bUseAccelerationForPaths = true;
}

bool UPaperNavMovementComponent::CanStartPathFollowing() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("CanStartPathFollowing"));
	return Super::CanStartPathFollowing();
}

bool UPaperNavMovementComponent::CanStopPathFollowing() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("CanStopPathFollowing"));
	return Super::CanStopPathFollowing();
}
/*
FBasedPosition UPaperNavMovementComponent::GetActorFeetLocationBased() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("GetActorFeetLocationBased"));
	return Super::GetActorFeetLocationBased();
}
*/
float UPaperNavMovementComponent::GetPathFollowingBrakingDistance(float MaxSpeed) const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("GetPathFollowingBrakingDistance: %f"), MaxSpeed);
	return Super::GetPathFollowingBrakingDistance(MaxSpeed);
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

bool UPaperNavMovementComponent::IsFlying() const
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("IsFlying"));
	return Super::IsFlying();
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

void UPaperNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// TODO this probably needs to be adjusted to ensure unit values
	InputVector = FVector2D(MoveVelocity.X, MoveVelocity.Z).ClampAxes(-1.f, 1.f);

	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("RequestDirectMove: %s; %s"), *MoveVelocity.ToString(), bForceMaxSpeed ? TEXT("true") : TEXT("false"));
}

void UPaperNavMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	// TODO this probably needs to be adjusted to ensure unit values
	InputVector = FVector2D(MoveInput.X, MoveInput.Z).ClampAxes(-1.f, 1.f);

	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("RequestPathMove: %s"), *MoveInput.ToString());
}

void UPaperNavMovementComponent::StopActiveMovement()
{
	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("StopActiveMovement"));
}

FVector2D const& UPaperNavMovementComponent::GetInputVector() const
{
	return InputVector;
}
