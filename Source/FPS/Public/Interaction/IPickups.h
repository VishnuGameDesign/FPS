// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickups.generated.h"

class AFPSPlayer;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPickups : public UInterface
{
	GENERATED_BODY()
};

class FPS_API IIPickups
{
	GENERATED_BODY()
	
public:
	virtual void Pickup() = 0;
	virtual void Drop() = 0;
}; 
