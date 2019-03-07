// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PaperActor.h"
#include "PaperFlipbookComponent.h"

// Sets default values
APaperActor::APaperActor(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, FlipbookComponent(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(FName(TEXT("PaperFlipbookComponent0")));

	RootComponent = FlipbookComponent;
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

}

void APaperActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

