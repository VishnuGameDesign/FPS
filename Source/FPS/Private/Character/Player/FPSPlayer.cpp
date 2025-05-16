// Copyright by Vishnu Suresh

#include "Character/Player/FPSPlayer.h"

#include "MovieSceneFwd.h"
#include "VectorTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnitConversion.h"

AFPSPlayer::AFPSPlayer()
{
	PlayerCam = CreateDefaultSubobject<UCameraComponent>("PlayerCam");
	PlayerCam->SetupAttachment(RootComponent);
	PlayerCam->bUsePawnControlRotation = true;
	
	FPSPlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerMesh");
	FPSPlayerMesh->SetupAttachment(PlayerCam);
	FPSPlayerMesh->SetOnlyOwnerSee(true);
	FPSPlayerMesh->bCastDynamicShadow = false;
	FPSPlayerMesh->CastShadow = false;

	Weapon->SetupAttachment(FPSPlayerMesh, WeaponSocketName);
	
	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AFPSPlayer::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// smooth crouching 
	if (bInitSmoothCrouch)
		CrouchToTargetHeight(CrouchedCapsuleHalfHeight, DeltaTime);
	else
		CrouchToTargetHeight(StandingCapsuleHeight, DeltaTime);

	// line tracing for wall run

	if (bCanWallRun)
	{
		FHitResult RightHit, LeftHit;
		
		AActor* RightWall = CheckWall(GetActorRightVector(), RightHit);
		AActor* LeftWall = CheckWall(-GetActorRightVector(), LeftHit);
	
		if (RightWall)
		{
			RunnableWall = RightWall;
			if (RunnableWall)
			{
				CheckFacingWallDirection(RightHit.Normal);
			}
		}
		else if (LeftWall)
		{
			RunnableWall = LeftWall;
			if (RunnableWall)
			{
				CheckFacingWallDirection(LeftHit.Normal);
			}
		}
	}
	
}

AActor* AFPSPlayer::CheckWall(const FVector& Direction, FHitResult& HitResult)
{
    const FVector TraceStart = GetActorLocation();
    const FVector TraceEnd = TraceStart + Direction * LineTraceDistance;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WallRun, Params))
    {
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, true, 1.f, 0, 1.f);
        if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
        {
            return HitResult.GetActor();
        }
    }
    else if (bIsRunningOnWall && !bIsJumpingOffWall)
    {
        StopRunningOnWall();
    }
    return nullptr;
}



void AFPSPlayer::CheckFacingWallDirection(const FVector& Normal)
{
	WallNormal = Normal;
	FacingDirection = FVector::DotProduct(GetActorRightVector(), WallNormal);
	if (FMath::Abs(FacingDirection) > 0.8f)
	{
		RunnableWall->RunOnWall(this, WallNormal, FacingDirection, WallRunSpeed);
	}
}

void AFPSPlayer::JumpOffWall()	
{
	bIsJumpingOffWall = true;
	JumpCurrentCount = JumpCurrentCount - 1;
	
	FVector Direction = WallNormal + FVector::UpVector * 2.0f;
	Direction.Normalize();

	FVector LaunchVelocity = Direction * JumpXForce + FVector::UpVector * JumpYForce;
	LaunchCharacter(LaunchVelocity, false, false);
	SetMovementState(EPlayerMovementState::WallJumping);
	bCanWallRun = false;

	StopRunningOnWall();
	GetWorldTimerManager().SetTimer(WallRunCooldownHandle, this, &AFPSPlayer::ResetWallRun, WallRunCooldownTime, false);
}

void AFPSPlayer::SetMovementState(EPlayerMovementState NewMovementState)
{
	if (PlayerMovementState != NewMovementState)
	{
		PlayerMovementState = NewMovementState;
	}
}

TObjectPtr<USkeletalMeshComponent> AFPSPlayer::GetPlayerMesh()
{
	return FPSPlayerMesh;
}

void AFPSPlayer::StopRunningOnWall()
{
	if (JumpMaxCount > MaxJumCount)
	{
		JumpMaxCount = MaxJumCount;
	}
	
	if (bIsRunningOnWall)
	{
		bIsRunningOnWall = false;
	}
}

void AFPSPlayer::StartSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFPSPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFPSPlayer::StartCrouch()
{
	bInitSmoothCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
}

void AFPSPlayer::StopCrouch()
{
	bInitSmoothCrouch = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFPSPlayer::ResetWallRun()
{
	bCanWallRun = true;
	bIsJumpingOffWall = false;
}

void AFPSPlayer::CrouchToTargetHeight(float TargetHeight, float Time)
{
	float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	float NewHeight = FMath::FInterpTo(CurrentCapsuleHalfHeight, TargetHeight, Time, CrouchInterpSpeed);
	
	if (!FMath::IsNearlyEqual(CurrentCapsuleHalfHeight, NewHeight, 0.1f))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight, true);
	}
}



