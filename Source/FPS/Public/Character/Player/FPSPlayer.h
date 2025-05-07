// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/FPSCharacterBase.h"
#include "Interaction/IRunnableWall.h"
#include "FPSPlayer.generated.h"

#define ECC_WallRun ECC_GameTraceChannel2
class UCameraComponent;

UCLASS()
class FPS_API AFPSPlayer : public AFPSCharacterBase
{
	GENERATED_BODY()
public:
	AFPSPlayer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void StartSprinting();
	void StopSprinting();
	void StartCrouch();
	void StopCrouch();
	void RunOnWall();
	void StopRunningOnWall();
	void JumpOffWall();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	TObjectPtr<USkeletalMeshComponent> FPSPlayerMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float StandingCapsuleHeight = 88.f;
	
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchedCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchInterpSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	bool bInitSmoothCrouch = false;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float MaxWallRunTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunSpeed = 800.f;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallJumpForce = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float DefaultGravityScale = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunGravityScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float LineTraceDistance = 5.f;
	
	UPROPERTY(VisibleAnywhere, Category= "Wall Run")
	FVector CurrentWallImpactNormal;

	UPROPERTY(BlueprintReadOnly, Category = "Wall Run")
	bool bIsRunningOnWall = false;
	
private:
	void CrouchToTargetHeight(float TargetHeight, float Time);
	void OnPlayerMoving();
	TScriptInterface<IIRunnableWall> CheckWall(const FVector& Direction, FHitResult& HitResult);
	void CheckFacingDir(const FVector& WallNormal);
	TScriptInterface<IIRunnableWall> RunnableWall;
	FTimerHandle WallRunTimer;
	float FacingDirection;
	bool bCanRunWall;
};
