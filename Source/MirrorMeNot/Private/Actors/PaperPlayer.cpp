// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperPlayer.h"
#include "Components/CapsuleComponent.h"
#include "EntityActor.h"

APaperPlayer::APaperPlayer(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, InputVector(FVector2D::ZeroVector)
{
}

void APaperPlayer::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnBeginEntityOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APaperPlayer::OnEndEntityOverlap);
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperPlayer::Jump);
	BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperPlayer::MoveLeft);
	BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperPlayer::MoveRight);
}

FVector2D APaperPlayer::GetInputVector() const
{
	return InputVector;
}

void APaperPlayer::Jump(bool const bPressed)
{
	InputVector.Y = bPressed ? 1.f : 0.f;
}

void APaperPlayer::MoveLeft(bool const bPressed)
{
	if (bPressed)
	{
		InputVector.X = -1.f;

		SetOrientation(InputVector.X);
	}
	else if (InputVector.X == -1.f)
	{
		InputVector.X = 0.f;
	}
}

void APaperPlayer::MoveRight(bool const bPressed)
{
	if (bPressed)
	{
		InputVector.X = 1.f;

		SetOrientation(InputVector.X);
	}
	else if (InputVector.X == 1.f)
	{
		InputVector.X = 0.f;
	}
}

void APaperPlayer::OnBeginEntityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult const& SweepResult)
{
	Cast<APaperEntity>
}

void APaperPlayer::OnEndEntityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
