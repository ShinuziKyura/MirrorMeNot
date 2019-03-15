// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PaperPawn.h"
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

/// APawn interface

	virtual void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) override;

/// APaperPawn interface

	virtual FVector2D GetInputVector() const override;

private:
	void JumpUp(bool const bPressed);

	void FallDown(bool const bPressed);

	void MoveLeft(bool const bPressed);

	void MoveRight(bool const bPressed);

	// Utility method to setup inputs // TODO RS maybe extract this to utility library
	template <class UserClass>
	static void BindAction(UInputComponent * PlayerInputComponent, FName const ActionName, UserClass * Object, typename TBaseDelegate<void, bool>::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
	{
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool const>>(ActionName, IE_Pressed, Object, Func, true);
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool const>>(ActionName, IE_Released, Object, Func, false);
	}
};
