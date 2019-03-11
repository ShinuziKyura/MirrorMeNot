// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperActor.h"
#include "Engine/CollisionProfile.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/WorldSettings.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogPaperActor)

const float APaperActor::MaxJumpDuration = .1f;
const FName APaperActor::PaperActor_ProfileName(TEXT("PaperActor"));

APaperActor::APaperActor(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, bDrawDebugTraces(false)
	, JumpMultiplier(3.5f)
	, MovementMultiplier(3.f)
	, MaxVelocityMultiplier(.3f)
	, ReferenceVelocity(0.f)
	, JumpDuration(0.f)
	, bIsFlying(EFlightState::None)
	, bIsMoving(EMovementDirection::None)
{
	PrimaryActorTick.bCanEverTick = true;

	PhysicsBodyComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent0"));
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent0"));

	SetRootComponent(PhysicsBodyComponent);

	PhysicsBodyComponent->SetSimulatePhysics(true);
	PhysicsBodyComponent->SetNotifyRigidBodyCollision(true);
	PhysicsBodyComponent->SetCollisionProfileName(PaperActor_ProfileName);
	
	FlipbookComponent->SetupAttachment(PhysicsBodyComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APaperActor::BeginPlay()
{
	Super::BeginPlay();

	ReferenceVelocity = -GetWorld()->GetGravityZ();
	
	GroundCollisionParams.AddIgnoredActor(this);
}

void APaperActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();

	// Process last frame trace
	FTraceDatum GroundCollision;
	if (World->QueryTraceData(GroundCollisionHandle, GroundCollision))
	{
		if (bDrawDebugTraces)
		{
			DrawDebugLine(World, GroundCollision.Start, GroundCollision.End, FColor::Green, false, 1.f, 0, 1.f);
		}

		if (GroundCollision.OutHits.Num())
		{
			JumpDuration = 0.f;
			bIsFlying = EFlightState::None;

			DrawDebugSphere(World, GroundCollision.OutHits.Last().ImpactPoint, 1.f, 16, FColor::Red, false, 1.f, 0, 1.f);
		}
	}

	// Clamp maximum speed
	auto LinearVelocity = PhysicsBodyComponent->GetPhysicsLinearVelocity();
	LinearVelocity.X = FMath::Clamp(LinearVelocity.X, -ReferenceVelocity * MaxVelocityMultiplier, ReferenceVelocity * MaxVelocityMultiplier);
	PhysicsBodyComponent->SetPhysicsLinearVelocity(LinearVelocity);

	auto NewAcceleration = FVector::ZeroVector;

	if (bIsFlying == EFlightState::Jumping)
	{
		NewAcceleration.Z += ReferenceVelocity * JumpMultiplier;

		JumpDuration += DeltaTime;
		bIsFlying = JumpDuration <= MaxJumpDuration ? EFlightState::Jumping : EFlightState::Falling;
	}

	switch (bIsMoving)
	{
		case EMovementDirection::Left:
			NewAcceleration.X += ReferenceVelocity * -MovementMultiplier;
			break;
		case EMovementDirection::Right:
			NewAcceleration.X += ReferenceVelocity * MovementMultiplier;
			break;
		case EMovementDirection::None:
			NewAcceleration.X += -LinearVelocity.X * MovementMultiplier;
			break;
	}

	PhysicsBodyComponent->AddForce(NewAcceleration, NAME_None, true);

	// Test for collision
	if (bIsFlying == EFlightState::Falling)
	{
		const auto Start = PhysicsBodyComponent->GetComponentLocation();
		const auto End = Start - FVector(0.f, 0.f, PhysicsBodyComponent->GetUnscaledCapsuleHalfHeight() + 1.f);

		if (bDrawDebugTraces)
		{
			GroundCollisionHandle = World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, ECC_Visibility, GroundCollisionParams);
		}
	}
}

// TODO remove this
void APaperActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperActor::JumpUp);
	BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperActor::MoveLeft);
	BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperActor::MoveRight);
}

void APaperActor::JumpUp(const bool bPressed)
{
	if (bPressed)
	{
		if (bIsFlying == EFlightState::None)
		{
			bIsFlying = EFlightState::Jumping;
		}
	}
	else if (bIsFlying == EFlightState::Jumping)
	{
		bIsFlying = EFlightState::Falling;
	}
}

void APaperActor::FallDown(const bool bPressed)
{
	if (bPressed)
	{
		bIsFlying = EFlightState::Falling;
		// TODO logic to fall through
	}
}

void APaperActor::MoveLeft(const bool bPressed)
{
	if (bPressed)
	{
		bIsMoving = EMovementDirection::Left;
	}
	else if (bIsMoving == EMovementDirection::Left)
	{
		bIsMoving = EMovementDirection::None;
	}
}

void APaperActor::MoveRight(const bool bPressed)
{
	if (bPressed)
	{
		bIsMoving = EMovementDirection::Right;
	}
	else if (bIsMoving == EMovementDirection::Right)
	{
		bIsMoving = EMovementDirection::None;
	}
}
