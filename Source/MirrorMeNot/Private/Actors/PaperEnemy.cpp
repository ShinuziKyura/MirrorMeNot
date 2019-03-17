// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperNavMovementComponent.h"

APaperEnemy::APaperEnemy(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, MovementComponent(ObjectInitializer.CreateDefaultSubobject<UPaperNavMovementComponent>(this, TEXT("PaperNavMovementComponent")))
{
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

UPawnMovementComponent* APaperEnemy::GetMovementComponent() const
{
	return MovementComponent;
}

bool APaperEnemy::CanJump() const
{
	return false;
}

FVector2D APaperEnemy::GetInputVector() const
{
	return MovementComponent->GetInputVector();
}

