// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperPawn.h"
#include "Engine/CollisionProfile.h"
#include "Engine/CustomCollisionProfile.h"
#include "Engine/CustomEngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogPaperPawn)

APaperPawn::APaperPawn(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, CollisionComponent(ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
	, MovementMultiplier(450.f)
	, JumpMultiplier(450.f)
	, MaximumJumpDuration(.2f)
	, bDrawDebugSweeps(false)
	, bDrawDebugHits(false)
	, bIsAerial(EAerialState::None) // TODO maybe should be Falling on start (is bStartPenetrating a problem?)
	, bIsMoving(EMovingDirection::None)
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
	// TODO add cycle counter

	Super::Tick(DeltaTime);

	const auto Input = GetInputVector();
	const auto VelocityZ = CollisionComponent->GetPhysicsLinearVelocity().Z;

	// Detect jumps
	if (FMath::IsNearlyEqual(Input.Y, 1.f) && CanJump())
	{
		bIsAerial = EAerialState::Jumping;
	}

	if (IsJumping() && CanJump())
	{
		JumpDuration += DeltaTime;
	}
	else
	{
		QueryLevelCollision();
	}

	CollisionComponent->SetPhysicsLinearVelocity(FVector(Input.X * MovementMultiplier, 0.f, IsJumping() ? Input.Y * JumpMultiplier : VelocityZ));
}

bool APaperPawn::IsJumping() const
{
	return bIsAerial == EAerialState::Jumping;
}

bool APaperPawn::IsFalling() const
{
	return bIsAerial == EAerialState::Falling;
}

bool APaperPawn::IsMoving() const
{
	return bIsMoving;
}

bool APaperPawn::CanJump() const
{
	return !IsFalling() && JumpDuration <= MaximumJumpDuration;
}

FVector2D APaperPawn::GetInputVector() const
{
	return FVector2D::ZeroVector;
}

void APaperPawn::QueryLevelCollision()
{
	const auto Point = CollisionComponent->GetComponentLocation() - FVector::UpVector * (CollisionComponent->GetUnscaledCapsuleHalfHeight() - CollisionComponent->GetUnscaledCapsuleRadius());
	const auto Direction = Point - FVector::UpVector;

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
	const bool bValidHit = TraceDatum.OutHits.Num();

	bIsAerial = !bValidHit ? EAerialState::Falling : EAerialState::None;

	if (bValidHit)
	{
		JumpDuration = 0.f;
	}

	UWorld * World = GetWorld();

	if (bDrawDebugSweeps)
	{
		DrawDebugSphere(World, TraceDatum.End, LevelCollisionShape.GetSphereRadius(), 16, bValidHit ? FColor::Red : FColor::Yellow, false, .5f, 0, .5f);
	}

	if (bDrawDebugHits && bValidHit)
	{
		const auto ImpactPoint = TraceDatum.OutHits.Last().ImpactPoint;

		DrawDebugLine(World, ImpactPoint + FVector::ForwardVector * 5.f, ImpactPoint - FVector::ForwardVector * 5.f, FColor::Red, false, 1.f, 0, 1.f);
		DrawDebugLine(World, ImpactPoint + FVector::RightVector * 5.f, ImpactPoint - FVector::RightVector * 5.f, FColor::Red, false, 1.f, 0, 1.f);
	}
}
