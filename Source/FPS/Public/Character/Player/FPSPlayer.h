// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/FPSCharacterBase.h"
#include "FPSPlayer.generated.h"

class UCameraComponent;

UCLASS()
class FPS_API AFPSPlayer : public AFPSCharacterBase
{
	GENERATED_BODY()
public:
	AFPSPlayer();
	virtual void Tick(float DeltaTime) override;
	void StartSprinting();
	void StopSprinting();
	void StartCrouch();
	void StopCrouch();

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumping = false;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	bool bInitSmoothCrouch = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	TObjectPtr<USkeletalMeshComponent> FPSPlayerMesh;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	float SprintSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	float StandingCapsuleHeight = 88.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	float CrouchedCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	float CrouchInterpSpeed = 1.f;

private:
	void CrouchToTargetHeight(float TargetHeight, float Time);

};
