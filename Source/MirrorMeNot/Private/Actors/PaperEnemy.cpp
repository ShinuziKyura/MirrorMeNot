// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

APaperEnemy::APaperEnemy(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, StopThreshold(64.f)
	, JumpThreshold(128.f)
	, DamageActor(nullptr)
	, InputVector(FVector2D::ZeroVector)
{
}

void APaperEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DamageActor = GetWorld()->SpawnActor<AEntityActor>(AEntityActor::StaticClass(), GetActorTransform(), SpawnParams);
//	DamageActor->Deactivate();
}

FVector2D APaperEnemy::GetInputVector() const
{
	return InputVector;
}

void APaperEnemy::SetInputVector(FVector const & Destination)
{
	FVector const Distance = Destination - GetActorLocation();

	SetOrientation(Distance.X);

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

	if (FMath::Abs(Distance.X) < JumpThreshold && Distance.Z > 16.f) // TODO RS adjust this if needed
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

void APaperEnemy::SetOrientation(float const InOrientation)
{
	Super::SetOrientation(InOrientation);

	Controller->SetControlRotation(FRotator(0.f, FMath::IsNegativeFloat(InOrientation) ? 180.f : 0.f, 0.f));
}
