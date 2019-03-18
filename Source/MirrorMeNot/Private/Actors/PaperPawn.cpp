// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperPawn.h"
#include "Engine/CollisionProfile.h"
#include "Engine/CustomCollisionProfile.h"
#include "Engine/CustomEngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY(LogPaperPawn)

DECLARE_CYCLE_STAT(TEXT("PaperPawn_Tick"), STAT_PaperPawn_Tick, STATGROUP_PaperPawn)

APaperPawn::APaperPawn(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, CollisionComponent(ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
	, MovementMultiplier(450.f)
	, JumpMultiplier(450.f)
	, MaximumJumpDuration(.2f)
	, bDrawDebugSweeps(false)
	, bDrawDebugHits(false)
	, bIsAerial(EVerticalMovement::None)
	, bIsMoving(EHorizontalMovement::None)
	, JumpDuration(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CollisionComponent);

	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetCollisionProfileName(UCustomCollisionProfile::PaperActor_ProfileName);

	FlipbookComponent->SetupAttachment(CollisionComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APaperPawn::BeginPlay()
{
	Super::BeginPlay();

	LevelCollisionObjectParams.AddObjectTypesToQuery(ECC_Level);
	LevelCollisionShape.SetSphere(CollisionComponent->GetUnscaledCapsuleRadius() - .1f); // Small offset so collisions against walls aren't detected
	LevelCollisionParams.AddIgnoredActor(this);
	LevelCollisionDelegate.BindUObject(this, &APaperPawn::LevelCollisionHandler);
}

void APaperPawn::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_PaperPawn_Tick)

	Super::Tick(DeltaTime);

	auto const Input = GetInputVector();
	auto const VelocityZ = CollisionComponent->GetPhysicsLinearVelocity().Z;

	// Change state machine

	if (CanJump())
	{
		bIsAerial = FMath::IsNearlyZero(Input.Y) ? EVerticalMovement::None : EVerticalMovement::Jumping;
	}
	else
	{
		bIsAerial = EVerticalMovement::Falling;
	}

	if (IsJumping())
	{
		JumpDuration += DeltaTime;
	}
	else
	{
		QueryLevelCollision();
	}

	switch (int32(FMath::Sign(Input.X)))
	{
	case -1:
		bIsMoving = EHorizontalMovement::Left;
		break;
	case 1:
		bIsMoving = EHorizontalMovement::Right;
		break;
	case 0:
	default:
		bIsMoving = EHorizontalMovement::None;
		break;
	}

	OnStateChanged.Broadcast(this);

	// Change components

	if (IsMoving())
	{
		FlipbookComponent->SetWorldRotation(FRotator(0.f, FMath::IsNegativeFloat(Input.X) ? 180.f : 0.f, 0.f));
	}

	CollisionComponent->SetPhysicsLinearVelocity(FVector(Input.X * MovementMultiplier, 0.f, IsJumping() ? Input.Y * JumpMultiplier : VelocityZ));
}

bool APaperPawn::IsJumping() const
{
	return bIsAerial == EVerticalMovement::Jumping;
}

bool APaperPawn::IsFalling() const
{
	return bIsAerial == EVerticalMovement::Falling;
}

bool APaperPawn::IsMoving() const
{
	return bIsMoving;
}

bool APaperPawn::CanJump() const
{
	return JumpDuration < MaximumJumpDuration;
}

FVector2D APaperPawn::GetInputVector() const
{
	return FVector2D::ZeroVector;
}

void APaperPawn::QueryLevelCollision()
{
	auto const Point = CollisionComponent->GetComponentLocation() - FVector::UpVector * (CollisionComponent->GetUnscaledCapsuleHalfHeight() - CollisionComponent->GetUnscaledCapsuleRadius());
	auto const Direction = Point - FVector::UpVector;

	GetWorld()->AsyncSweepByObjectType(
		EAsyncTraceType::Single, 
		Point,
		Direction,
		LevelCollisionObjectParams, 
		LevelCollisionShape,
		LevelCollisionParams, 
		&LevelCollisionDelegate
	);
}

void APaperPawn::LevelCollisionHandler(FTraceHandle const & TraceHandle, FTraceDatum & TraceDatum)
{
	UWorld * const World = GetWorld();

	bool const bValidHit = TraceDatum.OutHits.Num();

	JumpDuration = bValidHit ? 0.f : MaximumJumpDuration;

	if (bDrawDebugSweeps)
	{
		DrawDebugSphere(World, TraceDatum.End, LevelCollisionShape.GetSphereRadius(), 16, bValidHit ? FColor::Red : FColor::Yellow, false, .5f, 0, .5f);
	}

	if (bDrawDebugHits && bValidHit)
	{
		auto const ImpactPoint = TraceDatum.OutHits.Last().ImpactPoint;

		DrawDebugLine(World, ImpactPoint + FVector::ForwardVector * 5.f, ImpactPoint - FVector::ForwardVector * 5.f, FColor::Red, false, 1.f, 0, 1.f);
		DrawDebugLine(World, ImpactPoint + FVector::RightVector * 5.f, ImpactPoint - FVector::RightVector * 5.f, FColor::Red, false, 1.f, 0, 1.f);
	}
}
