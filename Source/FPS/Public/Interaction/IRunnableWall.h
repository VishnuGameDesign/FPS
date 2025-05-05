// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRunnableWall.generated.h"

class AFPSPlayer;

UINTERFACE(MinimalAPI)
class UIRunnableWall : public UInterface
{
	GENERATED_BODY()
};

class FPS_API IIRunnableWall
{
	GENERATED_BODY()

public:
	virtual void RunOnWall(AFPSPlayer* FPSPlayer, FVector Normal, float WallRunGravityScale) = 0;
	virtual void StopRunningOnWall(AFPSPlayer* FPSPlayer) = 0;
};
