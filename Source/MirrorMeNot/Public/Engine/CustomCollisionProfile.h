// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template <class>
struct TCustomCollisionProfile
{
	static const FName PaperActor_ProfileName;
	static const FName PaperIntangibleActor_ProfileName;
	static const FName PaperEntity_ProfileName;
	static const FName PaperLevel_ProfileName;
};

template <class Type>
const FName TCustomCollisionProfile<Type>::PaperActor_ProfileName{ TEXT("PaperActor") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperIntangibleActor_ProfileName{ TEXT("PaperIntangibleActor") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperEntity_ProfileName{ TEXT("PaperEntity") };
template <class Type>
const FName TCustomCollisionProfile<Type>::PaperLevel_ProfileName{ TEXT("PaperLevel") };

using UCustomCollisionProfile = TCustomCollisionProfile<void>;