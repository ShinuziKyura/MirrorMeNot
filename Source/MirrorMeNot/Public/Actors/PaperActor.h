// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "PaperActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPaperActor, Log, All)

UCLASS()
class MIRRORMENOT_API APaperActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaperActor(FObjectInitializer const& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

	template <class UserClass>
	void BindAction(UInputComponent* PlayerInputComponent, const FName ActionName, UserClass* Object, typename TBaseDelegate<void, bool>::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
	{
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>(ActionName, IE_Pressed, Object, Func, true);
		PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>(ActionName, IE_Released, Object, Func, false);
	}

protected:
	struct EMovementDirection
	{
		static const uint8 None = 0b00;
		static const uint8 Left = 0b01;
		static const uint8 Right = 0b10;
	};

	void JumpUp(const bool bPressed);

	void DropDown();

	void MoveLeft(const bool bPressed);

	void MoveRight(const bool bPressed);

	UFUNCTION()
	void OnComponentHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Actor")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paper Actor")
	class UPaperFlipbookComponent* FlipbookComponent;

	float JumpDuration;

	uint8 bIsJumping : 1;
	uint8 bIsFalling : 1;
	uint8 bIsMoving : 2;

	static const FName PaperActor_ProfileName;
};

