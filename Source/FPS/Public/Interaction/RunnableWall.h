// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "IRunnableWall.h"
#include "GameFramework/Actor.h"
#include "RunnableWall.generated.h"

class UBoxComponent;

UCLASS()
class FPS_API ARunnableWall : public AActor, public IIRunnableWall
{
	GENERATED_BODY()
	
public:	
	ARunnableWall();
	virtual void RunOnWall(AFPSPlayer* FPSPlayer, FVector Normal, float Direction, float Speed) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WallMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComponent;
};
