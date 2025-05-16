// Copyright by Vishnu Suresh

#include "Character/Base/FPSCharacterBase.h"

AFPSCharacterBase::AFPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->bCastDynamicShadow = false;
	Weapon->CastShadow = false;
}

void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

