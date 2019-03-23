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

	UFUNCTION(BlueprintCallable)
	void SetCanShiftWorlds(bool const bEnabled);

	void ShiftWorlds();

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldsShiftedDelegate, bool, bReversed);
	UPROPERTY(BlueprintAssignable)
	FOnWorldsShiftedDelegate OnWorldsShifted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePausedDelegate, bool, bActive);
	UPROPERTY(BlueprintAssignable)
	FOnGamePausedDelegate OnGamePaused;

private:
	TArray<AActor*> CachedTileMapActors;

	uint8 bCanShiftWorlds : 1;
	uint8 bIsShifted : 1;
	uint8 bIsPaused : 1;

};
