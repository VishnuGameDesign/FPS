// Copyright by Vishnu Suresh

#include "Interaction/RunnableWall.h"
#include "Character/Player/FPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARunnableWall::ARunnableWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("WallMesh");
	WallMesh->SetupAttachment(RootComponent);
}

void ARunnableWall::RunOnWall(AFPSPlayer* FPSPlayer, const FVector Normal, float WallRunGravityScale) 
{
	if (!FPSPlayer) return;

	float FacingDir = FVector::DotProduct(Normal, FPSPlayer->GetActorRightVector());
	if (FMath::Abs(FacingDir) > 0.8f)
	{
		FPSPlayer->GetCharacterMovement()->SetPlaneConstraintEnabled(true);
		FPSPlayer->GetCharacterMovement()->SetPlaneConstraintNormal(Normal);
		FPSPlayer->GetCharacterMovement()->GravityScale = WallRunGravityScale;
	}
}

void ARunnableWall::StopRunningOnWall(AFPSPlayer* FPSPlayer)
{
	if (!FPSPlayer) return;

	FPSPlayer->bIsRunningOnWall = false;
	FPSPlayer->GetCharacterMovement()->SetPlaneConstraintEnabled(false);
	FPSPlayer->GetCharacterMovement()->GravityScale = 1.0f;
}



