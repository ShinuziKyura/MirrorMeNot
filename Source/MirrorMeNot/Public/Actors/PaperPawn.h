// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorldCollision.h"
#include "PaperPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperPawn, Log, All)

UCLASS()
class MIRRORMENOT_API APaperPawn : public APawn
{
	GENERATED_BODY()

public:
	APaperPawn(FObjectInitializer const & ObjectInitializer);

/// AActor interface

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

/// APaperPawn interface

	bool IsJumping() const;

	bool IsFalling() const;

	bool CanJump() const;

	virtual FVector2D GetInputVector() const;

protected:
	struct EAerialMovement
	{
		static const uint8 None = 0b00;
		static const uint8 Jumping = 0b01;
		static const uint8 Falling = 0b10;
	};
	struct EMovementDirection
	{
		static const uint8 None = 0b00;
		static const uint8 Left = 0b01;
		static const uint8 Right = 0b10;
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Pawn")
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Pawn")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float MovementMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float JumpMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float MaximumJumpDuration;

	UPROPERTY(EditAnywhere, Category = "Paper Pawn | Debug")
	uint8 bDrawDebugSweeps : 1;
	UPROPERTY(EditAnywhere, Category = "Paper Pawn | Debug")
	uint8 bDrawDebugHits : 1;

	uint8 bIsAerial : 2;
	uint8 bIsMoving : 2;

private:
	void QueryLevelCollision();
	void LevelCollisionHandler(FTraceHandle const & TraceHandle, FTraceDatum & TraceDatum);

	FCollisionObjectQueryParams LevelCollisionObjectParams;
	FCollisionShape LevelCollisionShape;
	FCollisionQueryParams LevelCollisionParams;
	FTraceDelegate LevelCollisionDelegate;

	float JumpDuration;
};

