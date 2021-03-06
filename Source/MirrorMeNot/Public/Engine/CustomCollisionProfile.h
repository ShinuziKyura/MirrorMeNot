// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template <class>
struct TCustomCollisionProfile
{
	static const FName PaperPlayer_ProfileName;
	static const FName PaperEnemy_ProfileName;
	static const FName PaperIntangibleActor_ProfileName;
	static const FName PaperEntity_ProfileName;
	static const FName PaperLevel_ProfileName;
};

template <class Type>
const FName TCustomCollisionProfile<Type>::PaperPlayer_ProfileName{ TEXT("PaperPlayer") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperEnemy_ProfileName{ TEXT("PaperEnemy") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperIntangibleActor_ProfileName{ TEXT("PaperIntangibleActor") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperEntity_ProfileName{ TEXT("PaperEntity") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperLevel_ProfileName{ TEXT("PaperLevel") };

using UCustomCollisionProfile = TCustomCollisionProfile<void>;