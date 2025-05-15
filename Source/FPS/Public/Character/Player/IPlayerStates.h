// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlayerStates.generated.h"

DECLARE_MULTICAST_DELEGATE(FIdleSignature)
DECLARE_MULTICAST_DELEGATE(FWalkingSignature)
DECLARE_MULTICAST_DELEGATE(FJumpingSignature)
DECLARE_MULTICAST_DELEGATE(FCrouchingSignature)
DECLARE_MULTICAST_DELEGATE(FSprintingSignature)
DECLARE_MULTICAST_DELEGATE(FWallJumpingSignature)
DECLARE_MULTICAST_DELEGATE(FWallRunningSignature)

UINTERFACE(MinimalAPI)
class UIPlayerStates : public UInterface
{
	GENERATED_BODY()
};

class FPS_API IIPlayerStates
{
	GENERATED_BODY()

public:
	virtual FIdleSignature& OnIdle() = 0;
	virtual FWalkingSignature& OnWalking() = 0;
	virtual FJumpingSignature& OnJumping() = 0;
	virtual FCrouchingSignature& OnCrouching() = 0;
	virtual FSprintingSignature& OnSprinting() = 0;
	virtual FWallJumpingSignature& OnWallJumping() = 0;
	virtual FWallRunningSignature& OnWallRunning() = 0;
	
};
