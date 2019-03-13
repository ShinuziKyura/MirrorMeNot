// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "PaperNavMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperNavMovementComponent, Log, All)

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API UPaperNavMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	virtual void StopActiveMovement() override;

	virtual void StopMovementImmediately() override;

	virtual FBasedPosition GetActorFeetLocationBased() const override;

	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	virtual void RequestPathMove(const FVector& MoveInput) override;

	virtual bool CanStopPathFollowing() const override;

	virtual float GetPathFollowingBrakingDistance(float MaxSpeed) const override;

	virtual bool CanStartPathFollowing() const override;

	virtual bool IsCrouching() const override;

	virtual bool IsFalling() const override;

	virtual bool IsMovingOnGround() const override;

	virtual bool IsSwimming() const override;

	virtual bool IsFlying() const override;

};
