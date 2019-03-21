// Fill out your copyright notice in the Description page of Project Settings.

#include "MirrorMeNotGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PaperMirrorTileMapActor.h"

AMirrorMeNotGameModeBase::AMirrorMeNotGameModeBase(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, bCanShiftWorlds(false)
	, bIsReversed(false)
{
}

void AMirrorMeNotGameModeBase::ShiftWorlds()
{
	if (bCanShiftWorlds)
	{
		CacheTileMaps();

		for (auto const Actor : CachedTileMapActors)
		{
			if (auto const TileMapActor = Cast<APaperMirrorTileMapActor>(Actor))
			{
				TileMapActor->ShiftLayers();
			}
		}

		bIsReversed = !bIsReversed;

		OnWorldsShifted.Broadcast(bIsReversed);
	}
}

void AMirrorMeNotGameModeBase::SetCanShiftWorlds(bool const bEnabled)
{
	bCanShiftWorlds = bEnabled;
}

void AMirrorMeNotGameModeBase::CacheTileMaps()
{
	if (!CachedTileMapActors.Num())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaperMirrorTileMapActor::StaticClass(), CachedTileMapActors);
	}
}

