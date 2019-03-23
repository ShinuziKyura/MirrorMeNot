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
	LevelCollisionShape.SetBox(PhysicsComponent->GetUnscaledBoxExtent() * FVector(1.f, 1.f, 0.f) + FVector(-2.f, -2.f, 8.f));
	LevelCollisionParams.AddIgnoredActor(this);
	LevelCollisionDelegate.BindUObject(this, &APaperPawn::LevelCollisionHandler);
}

void APaperPawn::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_PaperPawn_Tick)

	Super::Tick(DeltaTime);

	auto const Input = GetInputVector();
	auto const VelocityZ = PhysicsComponent->GetPhysicsLinearVelocity().Z;

	// Update state machine

	if (CanJump())
	{
		bIsAerial = FMath::IsNearlyEqual(Input.Y, 1.f) ? EVerticalMovement::Jumping : EVerticalMovement::None;
	}
	else
	{
		bIsAerial = EVerticalMovement::Falling;
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

	OnStateUpdated.Broadcast(this);

	// Query level for collision

	if (IsJumping())
	{
		JumpDuration += DeltaTime;
	}
	else
	{
		QueryLevelCollision();
	}

	// Apply input to components

	SetOrientation(int32(FMath::Sign(Input.X)));

	PhysicsComponent->SetPhysicsLinearVelocity(FVector(Input.X * MovementMultiplier, 0.f, IsJumping() ? Input.Y * JumpMultiplier : VelocityZ));
}

FVector2D APaperPawn::GetInputVector() const
{
	return FVector2D::ZeroVector;
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

void APaperPawn::SetOrientation_Implementation(int32 const InOrientation)
{
	if (InOrientation)
	{
		FlipbookComponent->SetWorldRotation(FRotator(0.f, InOrientation == -1 ? 180.f : 0.f, 0.f));
	}
}

void APaperPawn::QueryLevelCollision()
{
	auto const Point = PhysicsComponent->GetComponentLocation();
	auto const Direction = Point - PhysicsComponent->GetUnscaledBoxExtent() * FVector::UpVector;

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
		DrawDebugBox(World, TraceDatum.End, LevelCollisionShape.GetBox(), bValidHit ? FColor::Red : FColor::Yellow, false, .5f, 0, .5f);
	}
}
