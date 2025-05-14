// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/FPSCharacterBase.h"
#include "Interaction/IRunnableWall.h"
#include "FPSPlayer.generated.h"

#define ECC_WallRun ECC_GameTraceChannel2

class UCameraComponent;

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	Idle,
	Walking,
	Sprinting,
	Crouching,
	Jumping,
	WallJumping,
	WallRunning,
};

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
	void JumpOffWall();

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumping = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bCanJump = true;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumpingOffWall = false;

	UPROPERTY(BlueprintReadOnly, Category = "Wall Run")
	bool bIsRunningOnWall = false;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	bool bInitSmoothCrouch = false;

	UPROPERTY(EditAnywhere, Category = "Player States")
	EPlayerMovementState PlayerMovementState { EPlayerMovementState::Idle };
protected:
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	TObjectPtr<USkeletalMeshComponent> FPSPlayerMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump")
	float JumpForce = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Wall Run")
	FVector WallNormal = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float LineTraceDistance = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float StandingCapsuleHeight = 88.f;
	
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchedCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchInterpSpeed = 1.f;

private:
	void CrouchToTargetHeight(float TargetHeight, float Time);
	AActor* CheckWall(const FVector& Direction, FHitResult& HitResult);
	void CheckFacingWallDirection(const FVector& Normal);
	void StopRunningOnWall();
	TScriptInterface<IIRunnableWall> RunnableWall;
	bool CanWallRun;
};
