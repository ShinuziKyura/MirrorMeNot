// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PaperNavMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperNavMovementComponent, Log, All)

// TODO read documentation:
// https://docs.unrealengine.com/en-us/Engine/Components/Navigation
// https://docs.unrealengine.com/en-us/Resources/ContentExamples/NavMesh

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

	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	virtual void StopActiveMovement() override;

/// UPaperNavMovementComponent interface

	FVector2D const& GetInputVector() const;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector2D InputVector;

};
