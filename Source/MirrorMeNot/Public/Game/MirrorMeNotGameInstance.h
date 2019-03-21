// Copyleft 2019-2019 SobTech. Some Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/InputComponent.h"
#include "MirrorMeNotGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORMENOT_API UMirrorMeNotGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Utility method to setup inputs
	template <class UserClass>
	static void BindAction(UInputComponent * PlayerInputComponent, FName const ActionName, UserClass * Object, typename TBaseDelegate<void, bool>::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
	{
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool const>>(ActionName, IE_Pressed, Object, Func, true);
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool const>>(ActionName, IE_Released, Object, Func, false);
	}

};
