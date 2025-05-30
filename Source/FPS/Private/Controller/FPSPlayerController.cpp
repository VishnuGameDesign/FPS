// Copyright by Vishnu Suresh

#include "Controller/FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/Player/FPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPSPlayerController::AFPSPlayerController()
{
	bReplicates = true;
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(PlayerInputMappingContext);
	check(WeaponsInputMappingContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
		Subsystem->AddMappingContext(WeaponsInputMappingContext, 0);
	}
}

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleNotMoving);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Look);
	// jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPSPlayerController::HandleJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleStopJumping);
	// crouch
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFPSPlayerController::InitCrouching);
	// sprint
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::HandleSprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleStopSprinting);
	// gun 
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Shoot);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Reload);
}

void AFPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovementState();
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(FPSCharacter->PlayerMovementState));
}

void AFPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	FPSCharacter = Cast<AFPSPlayer>(InPawn);
}

void AFPSPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	APawn* ControlledPawn = Cast<APawn>(GetPawn());
	
	if(ControlledPawn)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AFPSPlayerController::HandleNotMoving(const FInputActionValue& InputActionValue)
{
	FPSCharacter->PlayerMovementState = EPlayerMovementState::Idle;
}

void AFPSPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	FRotator Rotation = GetControlRotation();

	Rotation.Yaw += InputAxisVector.X;
	Rotation.Pitch += InputAxisVector.Y;

	Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, -50.0f, 50.0f) + InputAxisVector.Y;
	SetControlRotation(Rotation);
}

void AFPSPlayerController::HandleJump(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		if (FPSCharacter->bIsRunningOnWall)
		{
			FPSCharacter->JumpOffWall();
		}
		else 
		{
			FPSCharacter->bIsJumping = true;
			FPSCharacter->Jump();
		}
	}
}

void AFPSPlayerController::HandleStopJumping(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		FPSCharacter->StopJumping();
		FPSCharacter->bIsJumping = false;
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Idle;
	}
}

void AFPSPlayerController::InitCrouching(const FInputActionValue& InputActionValue)
{
	bCrouchedPressed = !bCrouchedPressed; 
	if (bCrouchedPressed)
	{
		HandleCrouch();
	}
	else
	{
		HandleUnCrouch();
	}
}

void AFPSPlayerController::HandleCrouch()
{
	if (FPSCharacter)
	{
		FPSCharacter->StartCrouch();
	}
}


void AFPSPlayerController::HandleUnCrouch()
{
	if (FPSCharacter)
	{
		FPSCharacter->StopCrouch();
	}
}

void AFPSPlayerController::HandleSprinting(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		FPSCharacter->StartSprinting();
	}
}


void AFPSPlayerController::HandleStopSprinting(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		FPSCharacter->StopSprinting();
	}
}

void AFPSPlayerController::HandlePickup(const FInputActionValue& InputActionValue)
{
	OnPickupPressed.Broadcast();
}

void AFPSPlayerController::HandleDrop(const FInputActionValue& InputActionValue)
{
	OnDropPressed.Broadcast();
}


void AFPSPlayerController::Shoot(const FInputActionValue& InputActionValue)
{
}

void AFPSPlayerController::Reload(const FInputActionValue& InputActionValue)
{
}

void AFPSPlayerController::UpdateMovementState()
{
	if (FPSCharacter->bIsJumpingOffWall)
	{
		OnPlayerWallJumping.Broadcast();
		FPSCharacter->PlayerMovementState = EPlayerMovementState::WallJumping;
	}
	else if (FPSCharacter->bIsRunningOnWall)
	{
		OnPlayerWallRunning.Broadcast();
		FPSCharacter->PlayerMovementState = EPlayerMovementState::WallRunning;
	}
	else if (FPSCharacter->GetCharacterMovement()->IsFalling() && !FPSCharacter->bIsJumpingOffWall)
	{
		OnPlayerFalling.Broadcast();
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Jumping;
	}
	else if (FPSCharacter->bIsSliding)
	{
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Sliding;
	}
	else if (FPSCharacter->bIsCrouching)
	{
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Crouching;
	}
	else if (FPSCharacter->bIsSprinting)
	{
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Sprinting;
	}
	else if (FPSCharacter->GetVelocity().SizeSquared() > 0.f)
	{
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Walking;
	}
	else
	{
		FPSCharacter->PlayerMovementState = EPlayerMovementState::Idle;
	}
}