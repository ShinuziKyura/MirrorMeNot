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

/// APaperEntity interface

	UFUNCTION(BlueprintPure, Category = "Paper Entity")
	EEntityType GetType() const;

	UFUNCTION(BlueprintPure, Category = "Paper Entity")
	float GetValue() const;

	UFUNCTION(BlueprintPure, Category = "Paper Entity")
	bool GetState() const;

	UFUNCTION(BlueprintCallable, Category = "Paper Entity")
	void SetState(bool const bEnabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Entity")
	class UBoxComponent * OverlapComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Entity")
	class USplineComponent * PathComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Entity")
	class UPaperSpriteComponent * SpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Entity")
	class UPaperFlipbookComponent * FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Entity", meta = (AllowPrivateAccess = "true"))
	EEntityType Type;
	// General-purpose value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Paper Entity", meta = (AllowPrivateAccess = "true"))
	float Value;

};
