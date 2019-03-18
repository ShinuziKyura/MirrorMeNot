// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PaperAIController.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API APaperAIController : public AAIController
{
	GENERATED_BODY()

public:
	APaperAIController(FObjectInitializer const & ObjectInitializer);

	virtual void SetInitialLocationAndRotation(const FVector& NewLocation, const FRotator& NewRotation) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

};
