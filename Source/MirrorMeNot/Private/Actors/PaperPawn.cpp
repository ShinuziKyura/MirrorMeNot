// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperPawn.h"
#include "Engine/CollisionProfile.h"
#include "Components/CapsuleComponent.h"
#include "PaperNavMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/WorldSettings.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogPaperPawn)

const float APaperPawn::MaxJumpDuration = .1f;
const FName APaperPawn::PaperPawn_ProfileName(TEXT("PaperPawn"));

APaperPawn::APaperPawn(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, bDrawDebugTraces(false)
	, JumpMultiplier(3.5f)
	, MovementMultiplier(3.f)
	, MaxVelocityMultiplier(.3f)
	, ReferenceVelocity(0.f)
	, JumpDuration(0.f)
	, bIsInAir(EAerialMovement::None)
	, bIsOnGround(EGroundMovement::None)
{
	PrimaryActorTick.bCanEverTick = true;

	PhysicsBodyComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent0"));
	MovementComponent = CreateDefaultSubobject<UPaperNavMovementComponent>(TEXT("PaperNavMovementComponent0"));
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent0"));

	SetRootComponent(PhysicsBodyComponent);

	PhysicsBodyComponent->SetSimulatePhysics(true);
	PhysicsBodyComponent->SetNotifyRigidBodyCollision(true);
	PhysicsBodyComponent->SetCollisionProfileName(PaperPawn_ProfileName);

	MovementComponent->SetUpdatedComponent(PhysicsBodyComponent);
	
	FlipbookComponent->SetupAttachment(PhysicsBodyComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APaperPawn::BeginPlay()
{
	Super::BeginPlay();

	ReferenceVelocity = -GetWorld()->GetGravityZ();
	
	GroundCollisionParams.AddIgnoredActor(this);
}

void APaperPawn::Tick(float DeltaTime)
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
			bIsInAir = EAerialMovement::None;

			DrawDebugSphere(World, GroundCollision.OutHits.Last().ImpactPoint, 1.f, 16, FColor::Red, false, 1.f, 0, 1.f);
		}
	}

	// Clamp maximum speed
	auto LinearVelocity = PhysicsBodyComponent->GetPhysicsLinearVelocity();
	LinearVelocity.X = FMath::Clamp(LinearVelocity.X, -ReferenceVelocity * MaxVelocityMultiplier, ReferenceVelocity * MaxVelocityMultiplier);
	PhysicsBodyComponent->SetPhysicsLinearVelocity(LinearVelocity);

	auto NewAcceleration = FVector::ZeroVector;

	if (bIsInAir == EAerialMovement::Jumping)
	{
		NewAcceleration.Z += ReferenceVelocity * JumpMultiplier;

		JumpDuration += DeltaTime;
		bIsInAir = JumpDuration <= MaxJumpDuration ? EAerialMovement::Jumping : EAerialMovement::Falling;
	}

	switch (bIsOnGround)
	{
		case EGroundMovement::Left:
			NewAcceleration.X += ReferenceVelocity * -MovementMultiplier;
			break;
		case EGroundMovement::Right:
			NewAcceleration.X += ReferenceVelocity * MovementMultiplier;
			break;
		case EGroundMovement::None:
			NewAcceleration.X += -LinearVelocity.X * MovementMultiplier;
			break;
	}

	PhysicsBodyComponent->AddForce(NewAcceleration, NAME_None, true);

	// Test for collision
	if (bIsInAir == EAerialMovement::Falling)
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
void APaperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperPawn::JumpUp);
	BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperPawn::MoveLeft);
	BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperPawn::MoveRight);
}

void APaperPawn::JumpUp(const bool bPressed)
{
	if (bPressed)
	{
		if (bIsInAir == EAerialMovement::None)
		{
			bIsInAir = EAerialMovement::Jumping;
		}
	}
	else if (bIsInAir == EAerialMovement::Jumping)
	{
		bIsInAir = EAerialMovement::Falling;
	}
}

void APaperPawn::FallDown(const bool bPressed)
{
	if (bPressed)
	{
		bIsInAir = EAerialMovement::Falling;
		// TODO logic to fall through
	}
}

void APaperPawn::MoveLeft(const bool bPressed)
{
	if (bPressed)
	{
		bIsOnGround = EGroundMovement::Left;
	}
	else if (bIsOnGround == EGroundMovement::Left)
	{
		bIsOnGround = EGroundMovement::None;
	}
}

void APaperPawn::MoveRight(const bool bPressed)
{
	if (bPressed)
	{
		bIsOnGround = EGroundMovement::Right;
	}
	else if (bIsOnGround == EGroundMovement::Right)
	{
		bIsOnGround = EGroundMovement::None;
	}
}
