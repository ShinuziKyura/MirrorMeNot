// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/PaperMirrorTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "ComponentReregisterContext.h"

/** Removes all components that use the specified sprite asset from their scenes for the lifetime of the class. */
class FTileMapReregisterContext
{
public:
	/** Initialization constructor. */
	FTileMapReregisterContext(UPaperTileMap* TargetAsset)
	{
		// Look at tile map components
		for (TObjectIterator<UPaperTileMapComponent> TileMapIt; TileMapIt; ++TileMapIt)
		{
			if (UPaperTileMapComponent* TestComponent = *TileMapIt)
			{
				if (TestComponent->TileMap == TargetAsset)
				{
					AddComponentToRefresh(TestComponent);
				}
			}
		}
	}

protected:
	void AddComponentToRefresh(UActorComponent* Component)
	{
		if (ComponentContexts.Num() == 0)
		{
			// wait until resources are released
			FlushRenderingCommands();
		}

		new (ComponentContexts) FComponentReregisterContext(Component);
	}

private:
	/** The recreate contexts for the individual components. */
	TIndirectArray<FComponentReregisterContext> ComponentContexts;
};

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

void APaperMirrorTileMapActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bIsReversed) // TODO fix this, not working correctly on editor
	{
		ShiftLayers();
	}
}

void APaperMirrorTileMapActor::ShiftLayers()
{
	auto const TileMap = GetRenderComponent()->TileMap;

	FTileMapReregisterContext ReregisterComponents(TileMap);

	TileMap->TileLayers = bIsReversed ? TileLayers : ReverseTileLayers;

	bIsReversed = !bIsReversed;
}
