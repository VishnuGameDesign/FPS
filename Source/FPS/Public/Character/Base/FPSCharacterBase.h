// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacterBase.generated.h"

UCLASS(Abstract)
class FPS_API AFPSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
