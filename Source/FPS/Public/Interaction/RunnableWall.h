// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "IRunnableWall.h"
#include "GameFramework/Actor.h"
#include "RunnableWall.generated.h"

UCLASS()
class FPS_API ARunnableWall : public AActor, public IIRunnableWall
{
	GENERATED_BODY()
	
public:	
	ARunnableWall();
	virtual void RunOnWall(AFPSPlayer* FPSPlayer, FVector Normal, float WallRunGravityScale) override;
	virtual auto StopRunningOnWall(AFPSPlayer* FPSPlayer) -> void override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WallMesh;
};
