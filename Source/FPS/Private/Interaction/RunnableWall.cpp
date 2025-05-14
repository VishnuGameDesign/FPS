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

void ARunnableWall::RunOnWall(AFPSPlayer* FPSPlayer, const FVector Normal, float Direction, float Speed) 
{
	if (!FPSPlayer) return;

	FPSPlayer->bCanJump = false;
	FPSPlayer->bIsRunningOnWall = true;
	
	const FVector LaunchVelocity = FVector::CrossProduct(Normal, FPSPlayer->GetActorUpVector()) * Speed * Direction;
	FPSPlayer->LaunchCharacter(LaunchVelocity, true, true);
}



