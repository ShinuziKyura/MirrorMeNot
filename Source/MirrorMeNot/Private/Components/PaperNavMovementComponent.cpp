// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperNavMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PaperPawn.h"

DEFINE_LOG_CATEGORY(LogPaperNavMovementComponent)

UPaperNavMovementComponent::UPaperNavMovementComponent(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, InputVector(FVector2D::ZeroVector)
{
}

void UPaperNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	InputVector = FVector2D(FMath::Sign(MoveVelocity.X), 0.f); // TODO This needs to be adjusted if we make enemies capable of jumping

	UE_LOG(LogPaperNavMovementComponent, Log, TEXT("RequestDirectMove: %s"), *MoveVelocity.ToString());
}

void UPaperNavMovementComponent::StopActiveMovement()
{
	Super::StopActiveMovement();

	InputVector = FVector2D::ZeroVector;
}

FVector2D const& UPaperNavMovementComponent::GetInputVector() const
{
	return InputVector;
}
