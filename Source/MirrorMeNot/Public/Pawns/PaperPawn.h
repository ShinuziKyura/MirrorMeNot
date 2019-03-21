// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorldCollision.h"
#include "PaperPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperPawn, Log, All)

DECLARE_STATS_GROUP(TEXT("PaperPawn"), STATGROUP_PaperPawn, STATCAT_Advanced)

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

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsJumping() const;

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsFalling() const;

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsMoving() const;

	virtual bool CanJump() const;

	virtual FVector2D GetInputVector() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedDelegate, APaperPawn*, InPawn);
	UPROPERTY(BlueprintAssignable)
	FOnStateChangedDelegate OnStateChanged;

protected:
	struct EVerticalMovement
	{
		static const uint8 None = 0b00;
		static const uint8 Jumping = 0b01;
		static const uint8 Falling = 0b10;
	};
	struct EHorizontalMovement
	{
		static const uint8 None = 0b00;
		static const uint8 Left = 0b01;
		static const uint8 Right = 0b10;
	};

	virtual void SetOrientation(float const InOrientation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Pawn")
	class UBoxComponent* PhysicsComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Pawn")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float MovementMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float JumpMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Pawn | Movement")
	float MaximumJumpDuration;
	float JumpDuration;

private:
	void QueryLevelCollision();
	void LevelCollisionHandler(FTraceHandle const & TraceHandle, FTraceDatum & TraceDatum);

	FCollisionObjectQueryParams LevelCollisionObjectParams;
	FCollisionShape LevelCollisionShape;
	FCollisionQueryParams LevelCollisionParams;
	FTraceDelegate LevelCollisionDelegate;

	UPROPERTY(EditAnywhere, Category = "Paper Pawn | Debug", meta = (AllowPrivateAccess = "true"))
	uint8 bDrawDebugSweeps : 1;

	uint8 bIsAerial : 2;
	uint8 bIsMoving : 2;

};

