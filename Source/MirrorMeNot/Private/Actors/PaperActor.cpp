// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperActor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"

// Sets default values
APaperActor::APaperActor(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, FlipbookComponent(nullptr)
	, bCanJump(true)
	, bCanDrop(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent0")));
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(FName(TEXT("PaperFlipbookComponent0")));
	FlipbookComponent->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void APaperActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APaperActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceToPoint = SphereComponent->GetClosestPointOnCollision(SphereComponent->GetComponentTransform().GetLocation(), ClosestPoint);
}

void APaperActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APaperActor::MoveUp()
{
	if (bCanJump)
	{
		FlipbookComponent->AddImpulse(FVector(0.f, 0.f, 50.f));
		bCanJump = false;
	}
}


