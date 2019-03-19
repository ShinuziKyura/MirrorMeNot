// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/PaperPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Actors/PaperEntity.h"
#include "Components/InputComponent.h"
#include "MirrorMeNotGameInstance.h"

APaperPlayer::APaperPlayer(FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
	, InputVector(FVector2D::ZeroVector)
	, Health(2.9f)
	, Invulnerable(-1.f)
{
}

void APaperPlayer::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnEntityOverlap);
}

void APaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNegativeFloat(Invulnerable))
	{
		Invulnerable -= DeltaTime;
	}
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UMirrorMeNotGameInstance::BindAction(PlayerInputComponent, TEXT("Up"), this, &APaperPlayer::Jump);
	UMirrorMeNotGameInstance::BindAction(PlayerInputComponent, TEXT("Left"), this, &APaperPlayer::MoveLeft);
	UMirrorMeNotGameInstance::BindAction(PlayerInputComponent, TEXT("Right"), this, &APaperPlayer::MoveRight);
}

FVector2D APaperPlayer::GetInputVector() const
{
	return InputVector;
}

void APaperPlayer::SetPlayerInputState(bool const bEnabled)
{
	if (bEnabled)
	{
		EnableInput(nullptr);
	}
	else
	{
		InputVector = FVector2D::ZeroVector;

		DisableInput(nullptr);
	}
}

bool APaperPlayer::IsInvulnerable() const
{
	return !FMath::IsNegativeFloat(Invulnerable);
}

void APaperPlayer::Jump(bool const bPressed)
{
	InputVector.Y = bPressed ? 1.f : 0.f;
}

void APaperPlayer::MoveLeft(bool const bPressed)
{
	if (bPressed)
	{
		InputVector.X = -1.f;

		SetOrientation(InputVector.X);
	}
	else if (InputVector.X == -1.f)
	{
		InputVector.X = 0.f;
	}
}

void APaperPlayer::MoveRight(bool const bPressed)
{
	if (bPressed)
	{
		InputVector.X = 1.f;

		SetOrientation(InputVector.X);
	}
	else if (InputVector.X == 1.f)
	{
		InputVector.X = 0.f;
	}
}

void APaperPlayer::OnEntityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult const& SweepResult)
{
	auto const Entity = Cast<APaperEntity>(OtherActor);

	switch (Entity->GetType())
	{
	case EEntityType::Damage:
		if (!IsInvulnerable())
		{
			if (!FMath::IsNegativeFloat(Health -= Entity->GetValue()))
			{
				Invulnerable = 3.f;
				OnDamageReceived.Broadcast();
			}
			else
			{
				OnFatalDamageReceived.Broadcast();
			}
		}
		break;
	case EEntityType::Collectable:
		OnCollectableFound.Broadcast(Entity);
		break;
	default:
		break;
	}
}
