// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/FPSCharacterBase.h"
#include "Interaction/IRunnableWall.h"
#include "FPSPlayer.generated.h"

class UCameraComponent;

UCLASS()
class FPS_API AFPSPlayer : public AFPSCharacterBase
{
	GENERATED_BODY()
public:
	AFPSPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	void StartSprinting();
	void StopSprinting();
	void StartCrouch();
	void StopCrouch();
	void WallJump();
	FVector GetWallJumpVelocity();

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumping = false;

	UPROPERTY(BlueprintReadOnly, Category = "Wall Run")
	bool bIsRunningOnWall = false;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	bool bInitSmoothCrouch = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	TObjectPtr<USkeletalMeshComponent> FPSPlayerMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float MaxWallRunTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunGravityScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallJumpForce = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float StandingCapsuleHeight = 88.f;
	
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchedCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchInterpSpeed = 1.f;

private:
	void CrouchToTargetHeight(float TargetHeight, float Time);

	TScriptInterface<IIRunnableWall> RunnableWall;

	bool CanWallRun;
	FVector WallNormal;
};
