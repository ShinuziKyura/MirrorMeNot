// Copyleft 2019-2019 SobTech. Some Rights Reserved.

#include "Components/PhysicsComponent.h"

UPhysicsComponent::UPhysicsComponent(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UPrimitiveComponent::SetSimulatePhysics(true);
	SetGenerateOverlapEvents(false);
}
