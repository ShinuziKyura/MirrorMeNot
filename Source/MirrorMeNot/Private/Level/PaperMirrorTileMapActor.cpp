// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/PaperMirrorTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"

APaperMirrorTileMapActor::APaperMirrorTileMapActor(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsReversed(false)
{
}

void APaperMirrorTileMapActor::BeginPlay()
{
	TileLayers = GetRenderComponent()->TileMap->TileLayers;
	ReverseTileLayers.Reserve(TileLayers.Num());

	for (auto const Layer : TileLayers)
	{
		ReverseTileLayers.EmplaceAt(0, Layer);
	}
}

void APaperMirrorTileMapActor::SwapLayers()
{
	GetRenderComponent()->TileMap->TileLayers = bIsReversed ? TileLayers : ReverseTileLayers;

	bIsReversed = !bIsReversed;
}
