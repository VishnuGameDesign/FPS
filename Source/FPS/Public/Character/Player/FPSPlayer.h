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
	void ResetWallRun();
	void JumpOffWall();
	void SetMovementState(EPlayerMovementState NewMovementState);
	TObjectPtr<USkeletalMeshComponent> GetPlayerMesh();

	
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = FName("WeaponHandSocket");

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumping = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Settings")
	bool bIsJumpingOffWall = false;

	UPROPERTY(BlueprintReadOnly, Category = "Wall Run")
	bool bIsRunningOnWall = false;

	UPROPERTY(EditAnywhere, Category = "Player Movement Settings")
	bool bInitSmoothCrouch = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player States")
	EPlayerMovementState PlayerMovementState { EPlayerMovementState::Idle };
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Wall Run")
	FVector WallNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Wall Run")
	float FacingDirection = 0.0f;
	
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
	float JumpXForce = 2.f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump")
	float JumpYForce = 2.f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump")
	int MaxJumCount = 2;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	float WallRunCooldownTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Wall Run")
	bool bCanWallRun = true;

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
	FTimerHandle WallRunCooldownHandle;
};
