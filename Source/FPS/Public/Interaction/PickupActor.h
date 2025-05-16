// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "IPickups.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS(Abstract)
class FPS_API APickupActor : public AActor, public IIPickups
{
	GENERATED_BODY()
	
public:	
	APickupActor();
	virtual void Pickup() override {}
	virtual void Drop() override {}
};
