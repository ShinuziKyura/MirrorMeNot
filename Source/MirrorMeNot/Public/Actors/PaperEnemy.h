// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PaperPawn.h"
#include "EntityActor.h"
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

	virtual void BeginPlay() override;

/// APaperPawn interface

	virtual FVector2D GetInputVector() const override;

	UFUNCTION(BlueprintCallable, Category = "Paper Enemy | Navigation")
	virtual void SetInputVector(FVector const & Destination);

	UFUNCTION(BlueprintCallable, Category = "Paper Enemy | Navigation")
	virtual void ResetInputVector();

protected:
	virtual void SetOrientation(float const InOrientation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Enemy | Navigation")
	float StopThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Enemy | Navigation")
	float JumpThreshold;

private:
	UPROPERTY()
	AEntityActor * DamageActor;

	FVector2D InputVector;

};
