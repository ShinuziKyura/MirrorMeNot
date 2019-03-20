// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperEnemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "PaperFlipbookComponent.h"

APaperEnemy::APaperEnemy(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, DamageComponent(ObjectInitializer.CreateDefaultSubobject<UChildActorComponent>(this, TEXT("ChildActorComponent")))
	, StopThreshold(64.f)
	, JumpThreshold(128.f)
	, InputVector(FVector2D::ZeroVector)
{
	DamageComponent->SetupAttachment(PhysicsComponent);
}

FVector2D APaperEnemy::GetInputVector() const
{
	return InputVector;
}

void APaperEnemy::SetInputVector(FVector2D const & NewInputVector)
{
	InputVector = NewInputVector;
}

void APaperEnemy::CalculateInputVector(FVector const & Destination)
{
	FVector const Distance = Destination - GetActorLocation();

	if (FMath::Abs(Distance.X) < StopThreshold) 
	{
		InputVector.X = 0.f;
	}
	else if (Distance.X < 0.f) 
	{
		InputVector.X = -1.f;
	}
	else 
	{
		InputVector.X = 1.f;
	}

	if (FMath::Abs(Distance.X) < JumpThreshold && Distance.Z > 32.f) // NOTE adjust this value if needed
	{
		InputVector.Y = 1.f;
	}
	else 
	{
		InputVector.Y = 0.f;
	}
}

void APaperEnemy::ResetInputVector()
{
	InputVector = FVector2D::ZeroVector;
}

bool APaperEnemy::IsWalking() const
{
	return IsMoving() && FMath::IsNearlyEqual(InputVector.Size(), .1f);
}

void APaperEnemy::SetOrientation(int32 const InOrientation)
{
	Super::SetOrientation(InOrientation);

	if (InOrientation)
	{
		// TODO convert this to a BlueprintNativeEvent and do this in blueprints
		FlipbookComponent->SetRelativeLocation(FVector(InOrientation * -10.f, 0.f, 30.f));
		DamageComponent->SetRelativeLocation(FVector(InOrientation * 20.f, 0.f, 0.f));

		Controller->SetControlRotation(FRotator(0.f, InOrientation == -1 ? 180.f : 0.f, 0.f));
	}
}
