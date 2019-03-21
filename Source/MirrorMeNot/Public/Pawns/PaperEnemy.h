// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/PaperPawn.h"
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

/// APaperPawn interface

	virtual FVector2D GetInputVector() const override;

/// APaperEnemy interface

	UFUNCTION(BlueprintCallable, Category = "Paper Enemy | Navigation")
	void SetInputVector(FVector2D const & NewInputVector);

	UFUNCTION(BlueprintCallable, Category = "Paper Enemy | Navigation")
	virtual void CalculateInputVector(FVector const & Destination);

	UFUNCTION(BlueprintCallable, Category = "Paper Enemy | Navigation")
	virtual void ResetInputVector();

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsWalking() const;

protected:
	virtual void SetOrientation_Implementation(int32 const InOrientation) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Enemy | Interaction")
	class UChildActorComponent* DamageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Enemy | Navigation")
	float StopThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Enemy | Navigation")
	float JumpThreshold;

private:
	FVector2D InputVector;

};
