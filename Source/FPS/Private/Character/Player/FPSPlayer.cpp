// Copyright by Vishnu Suresh

#include "Character/Player/FPSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AFPSPlayer::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInitSmoothCrouch)
	{
		CrouchToTargetHeight(CrouchedCapsuleHalfHeight, DeltaTime);
	}
	else
	{
		CrouchToTargetHeight(StandingCapsuleHeight, DeltaTime);
	}
}

void AFPSPlayer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	WallNormal = HitNormal;
	if (Other && Other->Implements<UIRunnableWall>())
	{
		RunnableWall = Other;
		RunnableWall->RunOnWall(this, HitNormal, WallRunGravityScale);
		bIsRunningOnWall = true;
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

void AFPSPlayer::WallJump()
{
	if (!RunnableWall || !bIsRunningOnWall) return;

	RunnableWall->StopRunningOnWall(this);
	LaunchCharacter(GetWallJumpVelocity(), true, true);
}

FVector AFPSPlayer::GetWallJumpVelocity()
{
	FVector WallJumpVelocity = WallNormal * WallJumpForce;
	return WallJumpVelocity;
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



