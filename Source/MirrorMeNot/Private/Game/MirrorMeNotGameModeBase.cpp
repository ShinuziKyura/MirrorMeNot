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

void AMirrorMeNotGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaperMirrorTileMapActor::StaticClass(), CachedTileMapActors);
}

void AMirrorMeNotGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bIsReversed)
	{
		bCanShiftWorlds = true;
		ShiftWorlds();
		bCanShiftWorlds = false;
	}
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

		bIsReversed = !bIsReversed;

		OnWorldsShifted.Broadcast(bIsReversed);
	}
}

void AMirrorMeNotGameModeBase::SetCanShiftWorlds(bool const bEnabled)
{
	bCanShiftWorlds = bEnabled;
}
