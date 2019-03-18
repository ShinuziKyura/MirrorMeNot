// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityActor.h"
#include "Components/BoxComponent.h"
#include "CustomCollisionProfile.h"
#include "Engine/CollisionProfile.h"
#include "PaperFlipbookComponent.h"

AEntityActor::AEntityActor(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, BoxComponent(ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent")))
	, FlipbookComponent(ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PaperFlipbookComponent")))
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(BoxComponent);

	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(false);
//	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName(UCustomCollisionProfile::PaperEntity_ProfileName);

	FlipbookComponent->SetupAttachment(BoxComponent);
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void AEntityActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEntityActor::Activate()
{
	BoxComponent->Activate(true);
	FlipbookComponent->Activate(true);
}

void AEntityActor::Deactivate()
{
	BoxComponent->Deactivate();
	FlipbookComponent->Deactivate();
}

void AEntityActor::SetBoxExtent(FVector InBoxExtent)
{
	BoxComponent->SetBoxExtent(InBoxExtent, true); // TODO what is this second parameter?
}

UPaperFlipbookComponent* AEntityActor::GetFlipbook() const
{
	return FlipbookComponent;
}

