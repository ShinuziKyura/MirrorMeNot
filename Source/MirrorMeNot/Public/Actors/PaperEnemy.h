// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PaperPawn.h"
#include "PaperEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API APaperEnemy : public APaperPawn
{
	GENERATED_BODY()

public:
	APaperEnemy(FObjectInitializer const& ObjectInitializer);

/// APawn interface

	virtual UPawnMovementComponent* GetMovementComponent() const override;

/// APaperPawn interface

	virtual FVector2D GetInputVector() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Enemy", meta = (ExposeFunctionCategories = "Components|Movement, Components|Movement|Planar, AI|Components|NavMovement"))
	class UPaperNavMovementComponent* MovementComponent;

};
