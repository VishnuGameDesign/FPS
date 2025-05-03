// Copyright by Vishnu Suresh

#include "FPSCharacterBase.h"

AFPSCharacterBase::AFPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

