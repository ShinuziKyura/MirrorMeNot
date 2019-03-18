// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntityActor.generated.h"

UCLASS()
class MIRRORMENOT_API AEntityActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEntityActor(FObjectInitializer const & ObjectInitializer);

	virtual void BeginPlay() override;

	void Activate();

	void Deactivate();

	void SetBoxExtent(FVector InBoxExtent);

	class UPaperFlipbookComponent * GetFlipbook() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Entity Actor")
	class UBoxComponent * BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Entity Actor")
	class UPaperFlipbookComponent * FlipbookComponent; // TODO maybe use a simple sprite

};
