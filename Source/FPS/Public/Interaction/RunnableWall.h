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
	virtual void RunOnWall(AFPSPlayer* FPSPlayer, FVector WallNormal, float Direction) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunGravityScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WallMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComponent;
};
