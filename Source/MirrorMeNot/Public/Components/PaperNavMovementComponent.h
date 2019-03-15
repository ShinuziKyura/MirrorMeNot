// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PaperNavMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperNavMovementComponent, Log, All)

/**
 * 
 */
UCLASS(HideFunctions = (PawnMovement))
class MIRRORMENOT_API UPaperNavMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UPaperNavMovementComponent(FObjectInitializer const& ObjectInitializer);

/// UNavMovementComponent interface

	virtual bool CanStartPathFollowing() const override;

	virtual bool CanStopPathFollowing() const override;

	//virtual FBasedPosition GetActorFeetLocationBased() const override;

	virtual float GetPathFollowingBrakingDistance(float MaxSpeed) const override;

	virtual bool IsCrouching() const override;

	virtual bool IsFalling() const override;

	virtual bool IsFlying() const override;

	virtual bool IsMovingOnGround() const override;

	virtual bool IsSwimming() const override;

	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	virtual void RequestPathMove(const FVector& MoveInput) override;

	virtual void StopActiveMovement() override;

/// UPaperNavMovementComponent interface

	FVector2D const& GetInputVector() const;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector2D InputVector;

};
