// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperAIController.h"

APaperAIController::APaperAIController(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void APaperAIController::SetInitialLocationAndRotation(const FVector& NewLocation, const FRotator& NewRotation)
{
	Super::SetInitialLocationAndRotation(NewLocation, NewRotation);
	
	SetControlRotation(NewRotation);
}

void APaperAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	if (auto const Pawn = GetPawn())
	{
		const FVector FocalPoint = GetFocalPoint();
		const float TargetPointX = FMath::Sign(FocalPoint.X - Pawn->GetActorLocation().X);

		if (FAISystem::IsValidLocation(FocalPoint) && !FMath::IsNearlyZero(TargetPointX))
		{
			SetControlRotation(FRotator(0.f, FMath::IsNegativeFloat(TargetPointX) ? 180.f : 0.f, 0.f));
		}
	}
}
