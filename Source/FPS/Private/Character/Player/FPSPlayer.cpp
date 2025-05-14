// Copyright by Vishnu Suresh

#include "Character/Player/FPSPlayer.h"

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

	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AFPSPlayer::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player State: %s"), *UEnum::GetValueAsString(PlayerMovementState)));	
	// smooth crouching 
	if (bInitSmoothCrouch)
		CrouchToTargetHeight(CrouchedCapsuleHalfHeight, DeltaTime);
	else
		CrouchToTargetHeight(StandingCapsuleHeight, DeltaTime);

	// line tracing for wall run
	FHitResult RightHit, LeftHit;

	AActor* RightWall = CheckWall(GetActorRightVector(), RightHit);
	AActor* LeftWall = CheckWall(-GetActorRightVector(), LeftHit);
	
	if (RightWall)
	{
		RunnableWall = RightWall;
		if (RunnableWall)
			CheckFacingWallDirection(RightHit.Normal);
	}
	else if (LeftWall)
		RunnableWall = LeftWall;
		if (RunnableWall)
			CheckFacingWallDirection(LeftHit.Normal);
}

AActor* AFPSPlayer::CheckWall(const FVector& Direction, FHitResult& HitResult)
{
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + Direction * LineTraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WallRun, Params))
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd,  HitResult.bBlockingHit ? FColor::Blue : FColor::Red, true, 1.f, 0, 1.f);
		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			return HitResult.GetActor();
		}
	}
	return nullptr;
}


void AFPSPlayer::CheckFacingWallDirection(const FVector& Normal)
{
	WallNormal = Normal;
	const float FacingDir = FVector::DotProduct(GetActorRightVector(), WallNormal);
	if (FMath::Abs(FacingDir) > 0.8f)
	{
		RunnableWall->RunOnWall(this, WallNormal, FacingDir, WallRunSpeed);
	}
	else
	{
		StopRunningOnWall();
	}
}

void AFPSPlayer::JumpOffWall()	
{
	UE_LOG(LogTemp, Warning, TEXT("Jumping off the wall"));
	StopRunningOnWall();
	bIsJumpingOffWall = true; 
	FVector PlayerCurrentVelocity = GetCharacterMovement()->Velocity;
	FVector LaunchVelocity = -WallNormal * PlayerCurrentVelocity * JumpForce;
	LaunchCharacter(LaunchVelocity, false, false);
}

void AFPSPlayer::StopRunningOnWall()
{
	bIsRunningOnWall = false;
	bCanJump = true;
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

void AFPSPlayer::CrouchToTargetHeight(float TargetHeight, float Time)
{
	float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	float NewHeight = FMath::FInterpTo(CurrentCapsuleHalfHeight, TargetHeight, Time, CrouchInterpSpeed);
	
	if (!FMath::IsNearlyEqual(CurrentCapsuleHalfHeight, NewHeight, 0.1f))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight, true);
	}
}



