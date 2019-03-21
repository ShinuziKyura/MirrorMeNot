// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/PaperPawn.h"
#include "Engine/CollisionProfile.h"
#include "Engine/CustomCollisionProfile.h"
#include "Engine/CustomEngineTypes.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogPaperPawn)

DECLARE_CYCLE_STAT(TEXT("PaperPawn_Tick"), STAT_PaperPawn_Tick, STATGROUP_PaperPawn)

APaperPawn::APaperPawn(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, PhysicsComponent(ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
	, MovementMultiplier(450.f)
	, JumpMultiplier(450.f)
	, MaximumJumpDuration(.2f)
	, JumpDuration(0.f)
	, bDrawDebugSweeps(false)
	, bIsAerial(EVerticalMovement::None)
	, bIsMoving(EHorizontalMovement::None)
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(PhysicsComponent);
	
	PhysicsComponent->SetSimulatePhysics(true);
	PhysicsComponent->SetNotifyRigidBodyCollision(true);
	PhysicsComponent->SetCollisionProfileName(UCustomCollisionProfile::PaperPlayer_ProfileName);

	FlipbookComponent->SetupAttachment(PhysicsComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APaperPawn::BeginPlay()
{
	Super::BeginPlay();

	LevelCollisionObjectParams.AddObjectTypesToQuery(ECC_Level);
	LevelCollisionShape.SetBox(PhysicsComponent->GetUnscaledBoxExtent() - FVector::ForwardVector * 4.f);
	LevelCollisionParams.AddIgnoredActor(this);
	LevelCollisionDelegate.BindUObject(this, &APaperPawn::LevelCollisionHandler);
}

void APaperPawn::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_PaperPawn_Tick)

	Super::Tick(DeltaTime);

	auto const Input = GetInputVector();
	auto const VelocityZ = PhysicsComponent->GetPhysicsLinearVelocity().Z;

	// Change state machine

	if (CanJump())
	{
		bIsAerial = FMath::IsNearlyEqual(Input.Y, 1.f) ? EVerticalMovement::Jumping : EVerticalMovement::None;
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

	PhysicsComponent->SetPhysicsLinearVelocity(FVector(Input.X * MovementMultiplier, 0.f, IsJumping() ? Input.Y * JumpMultiplier : VelocityZ));
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

void APaperPawn::SetOrientation(float const InOrientation)
{
	FlipbookComponent->SetWorldRotation(FRotator(0.f, FMath::IsNegativeFloat(InOrientation) ? 180.f : 0.f, 0.f));
}

void APaperPawn::QueryLevelCollision()
{
	auto const Point = PhysicsComponent->GetComponentLocation();
	auto const Direction = Point - FVector::UpVector * 4.f;

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
		DrawDebugBox(World, TraceDatum.Start, LevelCollisionShape.GetBox(), bValidHit ? FColor::Red : FColor::Yellow, false, .5f, 0, .5f);
	}
}
