// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperEntity.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Engine/CollisionProfile.h"
#include "CustomCollisionProfile.h"

APaperEntity::APaperEntity(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, BoxComponent(ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent")))
	, SpriteComponent(ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("PaperSpriteComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
	, Type(EEntityType::None)
	, Value(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(BoxComponent);

	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->SetCollisionProfileName(UCustomCollisionProfile::PaperEntity_ProfileName);
	BoxComponent->SetAutoActivate(false);

	SpriteComponent->SetupAttachment(BoxComponent);
	SpriteComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SpriteComponent->SetAutoActivate(false);

	FlipbookComponent->SetupAttachment(BoxComponent);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	FlipbookComponent->SetAutoActivate(false);
}

void APaperEntity::Activate(EEntityType InType, float InValue, FVector InBoxExtent)
{
	if (!BoxComponent->IsActive())
	{
		Type = InType;
		Value = InValue;
		BoxComponent->SetBoxExtent(InBoxExtent);

		BoxComponent->Activate(true);
		SpriteComponent->Activate(true);
		FlipbookComponent->Activate(true);
	}
}

void APaperEntity::Activate(FVector InBoxExtent)
{
	ensure(Type != EEntityType::None);

	if (!InBoxExtent.IsNearlyZero())
	{
		BoxComponent->SetBoxExtent(InBoxExtent);
	}

	BoxComponent->Activate(true);
	SpriteComponent->Activate(true);
	FlipbookComponent->Activate(true);
}

void APaperEntity::Deactivate()
{
	BoxComponent->Deactivate();
	SpriteComponent->Deactivate();
	FlipbookComponent->Deactivate();
}

UPaperSpriteComponent* APaperEntity::GetSprite() const
{
	return SpriteComponent;
}

UPaperFlipbookComponent* APaperEntity::GetFlipbook() const
{
	return FlipbookComponent;
}

EEntityType APaperEntity::GetType() const
{
	return Type;
}

float APaperEntity::GetValue() const
{
	return Value;
}
