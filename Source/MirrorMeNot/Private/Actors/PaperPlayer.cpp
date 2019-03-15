// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperPlayer.h"

APaperPlayer::APaperPlayer(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperPlayer::JumpUp);
	BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperPlayer::MoveLeft);
	BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperPlayer::MoveRight);
}

FVector2D APaperPlayer::GetInputVector() const
{
	auto Input = FVector2D::ZeroVector;
	
	switch (bIsMoving)
	{
		case EMovementDirection::Left:
			Input.X = -1.f;
			break;
		case EMovementDirection::Right:
			Input.X = 1.f;
			break;
		case EMovementDirection::None:
		default:
			break;
	}
	switch (bIsAerial)
	{
		case EAerialMovement::Jumping:
			Input.Y = 1.f;
			break;
		case EAerialMovement::Falling:
		case EAerialMovement::None:
		default:
			break;
	}

	return Input;
}

void APaperPlayer::JumpUp(bool const bPressed)
{
	if (bPressed)
	{
		if (bIsAerial == EAerialMovement::None)
		{
			bIsAerial = EAerialMovement::Jumping;
		}
	}
	else if (bIsAerial == EAerialMovement::Jumping)
	{
		bIsAerial = EAerialMovement::Falling;
	}
}

void APaperPlayer::FallDown(bool const bPressed)
{
	// TODO behaviour to fall through
	if (bPressed)
	{
		bIsAerial = EAerialMovement::Falling;
	}
}

void APaperPlayer::MoveLeft(bool const bPressed)
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

void APaperPlayer::MoveRight(bool const bPressed)
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
