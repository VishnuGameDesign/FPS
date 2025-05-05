// Copyright by Vishnu Suresh

#include "Interaction/RunnableWall.h"
#include "Character/Player/FPSPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARunnableWall::ARunnableWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("WallMesh");
	WallMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	BoxComponent->SetupAttachment(WallMesh);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WallRun, ECR_Block);
}

void ARunnableWall::RunOnWall(AFPSPlayer* FPSPlayer, const FVector WallNormal) 
{
	if (!FPSPlayer) return;

	UE_LOG(LogTemp, Warning, TEXT("RunOnWall"));
	float FacingDir = FVector::DotProduct(WallNormal, FPSPlayer->GetActorRightVector());
	if (FMath::Abs(FacingDir) > 0.8f)
	{
		FPSPlayer->GetCharacterMovement()->SetPlaneConstraintEnabled(true);
		FPSPlayer->GetCharacterMovement()->SetPlaneConstraintNormal(WallNormal);
		FPSPlayer->GetCharacterMovement()->GravityScale = WallRunGravityScale;
	}
}

void ARunnableWall::StopRunningOnWall(AFPSPlayer* FPSPlayer)
{
	if (!FPSPlayer) return;

	UE_LOG(LogTemp, Warning, TEXT("Stopping"));
	FPSPlayer->bIsRunningOnWall = false;
	FPSPlayer->GetCharacterMovement()->SetPlaneConstraintEnabled(false);
	FPSPlayer->GetCharacterMovement()->GravityScale = DefaultGravityScale;
}



