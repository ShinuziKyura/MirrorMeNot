// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperEntity.generated.h"

UENUM()
enum class EEntityType : uint8
{
	None,
	Damage,
	Collectable
};

UCLASS()
class MIRRORMENOT_API APaperEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	APaperEntity(FObjectInitializer const & ObjectInitializer);

/// APaperEntity interface // TODO check which of these methods should be exposed to BPs

	void Activate(EEntityType InType, float InValue, FVector InBoxExtent);

	void Activate(FVector InBoxExtent = FVector::ZeroVector);

	void Deactivate();

	class UPaperSpriteComponent * GetSprite() const;

	class UPaperFlipbookComponent * GetFlipbook() const;

	EEntityType GetType() const;

	float GetValue() const;

private:
	//TODO possibly add a trajectory/path for it to follow, plus lifetime functionality

	UPROPERTY(VisibleAnywhere, Category = "Paper Entity")
	class UBoxComponent * BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Paper Entity")
	class UPaperSpriteComponent * SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Paper Entity")
	class UPaperFlipbookComponent * FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Entity", meta = (AllowPrivateAccess = "true"))
	EEntityType Type;

	// General-purpose value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Entity", meta = (AllowPrivateAccess = "true"))
	float Value;

};
