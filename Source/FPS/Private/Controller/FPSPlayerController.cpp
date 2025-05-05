// Copyright by Vishnu Suresh


#include "Controller/FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/Player/FPSPlayer.h"

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
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Look);
	// jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::HandleJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleStopJumping);
	// crouch
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::HandleCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleUnCrouch);
	// sprint
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::HandleSprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFPSPlayerController::HandleStopSprinting);
	// gun 
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Shoot);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Reload);
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
		FPSCharacter->bIsJumping = true;
		FPSCharacter->Jump();
		if (FPSCharacter->bIsRunningOnWall)
		{
			FPSCharacter->WallJump();
		}
	}
}

void AFPSPlayerController::HandleStopJumping(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		FPSCharacter->StopJumping();
		FPSCharacter->bIsJumping = false;
	}
}

void AFPSPlayerController::HandleCrouch(const FInputActionValue& InputActionValue)
{
	if (FPSCharacter)
	{
		FPSCharacter->StartCrouch();
	}
}


void AFPSPlayerController::HandleUnCrouch(const FInputActionValue& InputActionValue)
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


void AFPSPlayerController::Shoot(const FInputActionValue& InputActionValue)
{
}

void AFPSPlayerController::Reload(const FInputActionValue& InputActionValue)
{
}




