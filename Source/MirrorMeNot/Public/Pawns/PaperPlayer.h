// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/PaperPawn.h"
#include "PaperPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API APaperPlayer : public APaperPawn
{
	GENERATED_BODY()

public:
	APaperPlayer(FObjectInitializer const & ObjectInitializer);

/// AActor interface

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

/// APawn interface

	virtual void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) override;

/// APaperPawn interface

	virtual FVector2D GetInputVector() const override;

/// APaperPlayer interface

	UFUNCTION(BlueprintCallable, Category = "Paper Player | Input")
	void SetPlayerInputState(bool const bEnabled);

	UFUNCTION(BlueprintPure, Category = "Paper Player | Status")
	bool IsInvulnerable() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageReceivedDelegate);
	UPROPERTY(BlueprintAssignable)
	FOnDamageReceivedDelegate OnDamageReceived;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFatalDamageReceivedDelegate);
	UPROPERTY(BlueprintAssignable)
	FOnFatalDamageReceivedDelegate OnFatalDamageReceived;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectableFoundDelegate, AActor*, InActor);
	UPROPERTY(BlueprintAssignable)
	FOnCollectableFoundDelegate OnCollectableFound;

private:
	void Jump(bool const bPressed);

	void MoveLeft(bool const bPressed);

	void MoveRight(bool const bPressed);

	UFUNCTION()
	void OnEntityOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult const & SweepResult);

	FVector2D InputVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Player | Status", meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Player | Status", meta = (AllowPrivateAccess = "true"))
	float Invulnerable;

};
