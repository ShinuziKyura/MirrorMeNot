// Fill out your copyright notice in the Description page of Project Settings.

#include "MirrorMeNotGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PaperMirrorTileMapActor.h"

AMirrorMeNotGameModeBase::AMirrorMeNotGameModeBase(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, bCanShiftWorlds(false)
	, bIsShifted(false)
{
}

void AMirrorMeNotGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaperMirrorTileMapActor::StaticClass(), CachedTileMapActors);
}

void AMirrorMeNotGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bIsShifted)
	{
		bCanShiftWorlds = true;

		ShiftWorlds();

		bCanShiftWorlds = false;
	}
}

void AMirrorMeNotGameModeBase::SetCanShiftWorlds(bool const bEnabled)
{
	bCanShiftWorlds = bEnabled;
}

void AMirrorMeNotGameModeBase::ShiftWorlds()
{
	if (bCanShiftWorlds)
	{
		for (auto const Actor : CachedTileMapActors)
		{
			if (auto const TileMapActor = Cast<APaperMirrorTileMapActor>(Actor))
			{
				TileMapActor->ShiftLayers();
			}
		}

		bIsShifted = !bIsShifted;

		OnWorldsShifted.Broadcast(bIsShifted);
	}
}

void AMirrorMeNotGameModeBase::PauseGame()
{
	bIsPaused = !bIsPaused;

	UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);

	OnGamePaused.Broadcast(bIsPaused);
}
