// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperActor.h"
#include "Engine/CollisionProfile.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/WorldSettings.h"

DEFINE_LOG_CATEGORY(LogPaperActor)

const FName APaperActor::PaperActor_ProfileName(TEXT("PaperActor"));

APaperActor::APaperActor(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, JumpDuration(0.0f)
	, bIsJumping(false)
	, bIsFalling(false)
	, bIsMoving(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent0"));
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent0"));

	SetRootComponent(SphereComponent);

	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->SetCollisionProfileName(PaperActor_ProfileName);
	
	FlipbookComponent->SetupAttachment(SphereComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

// Called when the game starts or when spawned
void APaperActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentHit.AddDynamic(this, &APaperActor::OnComponentHitHandler);
}

// Called every frame
void APaperActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO adjust these values according to GetWorld()->GetGravityZ()
	
	// Clamp maximum speed
	auto LinearVelocity = SphereComponent->GetPhysicsLinearVelocity();
	LinearVelocity.X = FMath::Clamp(LinearVelocity.X, -370.f * 2.f, 370.f * 2.f);
	SphereComponent->SetPhysicsLinearVelocity(LinearVelocity);

	if (bIsJumping)
	{
		SphereComponent->AddForce(FVector(0.f, 0.f, 9800.f), NAME_None, true);
		JumpDuration += DeltaTime;
		bIsJumping = JumpDuration <= .1f;
	}

	switch (bIsMoving)
	{
		case EMovementDirection::Left:
			SphereComponent->AddForce(FVector(-3700.f * 2.f, 0.f, 0.f), NAME_None, true);
			break;
		case EMovementDirection::Right:
			SphereComponent->AddForce(FVector(3700.f * 2.f, 0.f, 0.f), NAME_None, true);
			break;
		case EMovementDirection::None:
			if (FMath::IsNearlyZero(JumpDuration)) // TODO preference regarding this behaviour
			{
				SphereComponent->AddImpulse(FVector(-LinearVelocity.X * 10.f, 0.f, 0.f));
			}
			break;
	}
	
//	UE_LOG(LogPaperActor, Log, TEXT("%s"), *LinearVelocity.ToString())
}

void APaperActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// TODO remove this
	BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperActor::JumpUp);
	BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperActor::MoveLeft);
	BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperActor::MoveRight);
}

void APaperActor::JumpUp(const bool bPressed)
{
	bIsJumping = bPressed;
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

void APaperActor::OnComponentHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// TODO proper checking ground collision (ray traces sound good)
	JumpDuration = 0.f;

	auto LinearVelocity = SphereComponent->GetPhysicsLinearVelocity();
	LinearVelocity.Z = 0.f;
	SphereComponent->SetPhysicsLinearVelocity(LinearVelocity);

	UE_LOG(LogPaperActor, Log, TEXT("That's a hit!"));
}
