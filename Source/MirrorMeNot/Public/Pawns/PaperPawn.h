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

	// Gets vector with input information for each tick, should be overridden and provided with appropriate implementations
	virtual FVector2D GetInputVector() const;

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsJumping() const;

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsFalling() const;

	UFUNCTION(BlueprintPure, Category = "Paper Pawn | Movement")
	bool IsMoving() const;

	// Queries whether this pawn is currently able to jump, may be overridden to impose different (stronger or weaker) constraints
	virtual bool CanJump() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateUpdatedDelegate, APaperPawn*, InPawn);
	UPROPERTY(BlueprintAssignable)
	FOnStateUpdatedDelegate OnStateUpdated;

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

	// Sets orientation for relevant components, SetOrientation_Implementation may be overridden so derived classes' components can be adjusted as well
	UFUNCTION(BlueprintNativeEvent, Category = "Paper Pawn")
	void SetOrientation(int32 const InOrientation);
	
	virtual void SetOrientation_Implementation(int32 const InOrientation);

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

