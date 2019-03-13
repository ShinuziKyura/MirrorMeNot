// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorldCollision.h"
#include "Components/InputComponent.h"
#include "PaperPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperPawn, Log, All)

UCLASS()
class MIRRORMENOT_API APaperPawn : public APawn // TODO rename it to APaperPawn ?
{
	GENERATED_BODY()

public:
	APaperPawn(FObjectInitializer const& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

	// Utility method to setup inputs
	template <class UserClass>
	void BindAction(UInputComponent* PlayerInputComponent, const FName ActionName, UserClass* Object, typename TBaseDelegate<void, bool>::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
	{
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>(ActionName, IE_Pressed, Object, Func, true);
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>(ActionName, IE_Released, Object, Func, false);
	}

protected:
	struct EAerialMovement
	{
		static const uint8 None = 0b00;
		static const uint8 Jumping = 0b01;
		static const uint8 Falling = 0b10;
	};
	struct EGroundMovement
	{
		static const uint8 None = 0b00;
		static const uint8 Left = 0b01;
		static const uint8 Right = 0b10;
	};

	void JumpUp(const bool bPressed);

	void FallDown(const bool bPressed);

	void MoveLeft(const bool bPressed);

	void MoveRight(const bool bPressed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Actor")
	class UCapsuleComponent* PhysicsBodyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Actor")
	class UPaperNavMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Actor")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, Category = "Paper Actor | Debug")
	bool bDrawDebugTraces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Actor | Movement")
	float JumpMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Actor | Movement")
	float MovementMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper Actor | Movement")
	float MaxVelocityMultiplier;

	float ReferenceVelocity;
	float JumpDuration;

	FTraceHandle GroundCollisionHandle;
	FCollisionQueryParams GroundCollisionParams;

	uint8 bIsInAir : 2;
	uint8 bIsOnGround : 2;

	static const float MaxJumpDuration;
	static const FName PaperPawn_ProfileName;
};

