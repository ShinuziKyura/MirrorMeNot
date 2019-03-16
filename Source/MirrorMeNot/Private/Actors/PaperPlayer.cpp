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
		case EMovingDirection::Left:
			Input.X = -1.f;
			break;
		case EMovingDirection::Right:
			Input.X = 1.f;
			break;
		case EMovingDirection::None:
		default:
			break;
	}
	switch (bIsAerial)
	{
		case EAerialState::Jumping:
			Input.Y = 1.f;
			break;
		case EAerialState::Falling:
		case EAerialState::None:
		default:
			break;
	}

	return Input;
}

void APaperPlayer::JumpUp(bool const bPressed)
{
	if (bPressed)
	{
		if (bIsAerial == EAerialState::None)
		{
			bIsAerial = EAerialState::Jumping;
		}
	}
	else if (bIsAerial == EAerialState::Jumping)
	{
		bIsAerial = EAerialState::Falling;
	}
}

void APaperPlayer::FallDown(bool const bPressed)
{
	// TODO behaviour to fall through
	if (bPressed)
	{
		bIsAerial = EAerialState::Falling;
	}
}

void APaperPlayer::MoveLeft(bool const bPressed)
{
	if (bPressed)
	{
		bIsMoving = EMovingDirection::Left;
	}
	else if (bIsMoving == EMovingDirection::Left)
	{
		bIsMoving = EMovingDirection::None;
	}
}

void APaperPlayer::MoveRight(bool const bPressed)
{
	if (bPressed)
	{
		bIsMoving = EMovingDirection::Right;
	}
	else if (bIsMoving == EMovingDirection::Right)
	{
		bIsMoving = EMovingDirection::None;
	}
}
