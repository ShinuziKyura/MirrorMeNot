// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "PaperMirrorTileMapActor.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API APaperMirrorTileMapActor : public APaperTileMapActor
{
	GENERATED_BODY()

public:
	APaperMirrorTileMapActor(FObjectInitializer const& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void ShiftLayers();

private:
	TArray<class UPaperTileLayer*> TileLayers;
	TArray<class UPaperTileLayer*> ReverseTileLayers;

	bool bIsReversed;

};
