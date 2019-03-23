// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperEntity.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Engine/CollisionProfile.h"
#include "CustomCollisionProfile.h"

APaperEntity::APaperEntity(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, OverlapComponent(ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent")))
	, PathComponent(ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("SplineComponent")))
	, SpriteComponent(ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("PaperSpriteComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
	, Type(EEntityType::None)
	, Value(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(OverlapComponent);
	
	OverlapComponent->SetGenerateOverlapEvents(false);
	OverlapComponent->SetCollisionProfileName(UCustomCollisionProfile::PaperEntity_ProfileName);

	PathComponent->SetupAttachment(OverlapComponent);
	PathComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SpriteComponent->SetupAttachment(OverlapComponent);
	SpriteComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	FlipbookComponent->SetupAttachment(OverlapComponent);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

EEntityType APaperEntity::GetType() const
{
	return Type;
}

float APaperEntity::GetValue() const
{
	return Value;
}

bool APaperEntity::GetState() const
{
	return OverlapComponent->GetGenerateOverlapEvents();
}

void APaperEntity::SetState(bool const bEnabled)
{
	OverlapComponent->SetGenerateOverlapEvents(bEnabled);
}
