// Copyright by Vishnu Suresh

#include "Character/Player/FPSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/FPSPlayerController.h"
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

void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AFPSPlayerController*  FPSPlayerController = Cast<AFPSPlayerController>(PlayerController))
		{
			FPSPlayerController->OnCharacterMoving().AddUObject(this, &AFPSPlayer::OnPlayerMoving);
		}
	}
}

void AFPSPlayer::OnPlayerMoving()
{
	if (bCanRunWall)
	{
		RunOnWall();
	}
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
	FHitResult RightHit, LeftHit;
	   
	TScriptInterface<IIRunnableWall> RightWall = CheckWall(GetActorRightVector(), RightHit);
	TScriptInterface<IIRunnableWall> LeftWall = CheckWall(-GetActorRightVector(), LeftHit);
	
	if (RightWall)
	{
		CheckFacingDir(RightHit.Normal);
	}
	else if (LeftWall)
	{
		CheckFacingDir(LeftHit.Normal);
	}
}

TScriptInterface<IIRunnableWall> AFPSPlayer::CheckWall(const FVector& Direction, FHitResult& HitResult)
{
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + Direction * LineTraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WallRun, Params))
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd,  HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 10.f, 0, 1.f);
		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			CurrentWallImpactNormal = HitResult.ImpactNormal;
			RunnableWall = HitResult.GetActor();
			return RunnableWall;
		}
	}
	return nullptr;
}


void AFPSPlayer::CheckFacingDir(const FVector& WallNormal)
{
	FacingDirection = FVector::DotProduct(GetActorRightVector(), WallNormal);
	if (FMath::Abs(FacingDirection) > 0.5f)
	{
		if (!bIsRunningOnWall)
		{
			bCanRunWall = true;
		}
	}
	else
	{
		bCanRunWall = false;
	}
}

void AFPSPlayer::RunOnWall()
{
	bCanRunWall = false;
	bIsRunningOnWall = true;
	UE_LOG(LogTemp, Display, TEXT("RunOnWall"));
	GetCharacterMovement()->GravityScale = WallRunGravityScale;
	GetWorldTimerManager().SetTimer(WallRunTimer, this, &AFPSPlayer::StopRunningOnWall, MaxWallRunTime, false);

	// sticking the player to the wall
	FVector WallNormal = CurrentWallImpactNormal.GetSafeNormal();
	FVector Distance = WallNormal - GetActorLocation();
	FVector LaunchVelocity = Distance.Normalize() * WallNormal;
	LaunchCharacter(LaunchVelocity, true, true);
	
	
	// sending the character move in the forward direction whilst wall running
	float Direction = WallRunSpeed * FacingDirection;
	FVector ForwardLaunchVelocity = FVector::CrossProduct(WallNormal, FVector::UpVector) * Direction;
	LaunchCharacter(ForwardLaunchVelocity, true, false);
}

void AFPSPlayer::StopRunningOnWall()
{
	GetWorldTimerManager().ClearTimer(WallRunTimer);
	
	bIsRunningOnWall = false;
	UE_LOG(LogTemp, Display, TEXT("StopRunningOnWall"));
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
}


void AFPSPlayer::JumpOffWall()
{
	FVector LaunchDir = CurrentWallImpactNormal + FVector::UpVector * 0.2f;
	LaunchCharacter(LaunchDir * WallJumpForce, false, false);
	StopRunningOnWall();
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




