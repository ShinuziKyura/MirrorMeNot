// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PaperActor.generated.h"

UCLASS()
class MIRRORMENOT_API APaperActor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaperActor(FObjectInitializer const& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

private:
	class UPaperFlipbookComponent* FlipbookComponent;

	void MoveUp();

	void MoveDown();

	void MoveLeft();

	void MoveRight();

};
