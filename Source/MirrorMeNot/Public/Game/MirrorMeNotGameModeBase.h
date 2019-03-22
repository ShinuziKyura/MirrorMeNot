// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MirrorMeNotGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API AMirrorMeNotGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMirrorMeNotGameModeBase(FObjectInitializer const& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ShiftWorlds();

	UFUNCTION(BlueprintCallable)
	void SetCanShiftWorlds(bool const bEnabled);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldsShiftedDelegate, bool, bReversed);
	UPROPERTY(BlueprintAssignable)
	FOnWorldsShiftedDelegate OnWorldsShifted;

private:
	void CacheTileMaps();

	TArray<AActor*> CachedTileMapActors;

	bool bCanShiftWorlds;
	bool bIsReversed;

};
