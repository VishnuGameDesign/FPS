// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class AFPSPlayer;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerWallRunning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerWallJumping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerFalling);

UCLASS(Blueprintable)
class FPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFPSPlayerController();

	UPROPERTY(BlueprintReadOnly, Category = "FPS Character")
	TObjectPtr<AFPSPlayer> FPSCharacter;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	
	void Move(const FInputActionValue& InputActionValue);
	void HandleNotMoving(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void HandleJump(const FInputActionValue& InputActionValue);
	void HandleStopJumping(const FInputActionValue& InputActionValue);
	void HandleCrouch(const FInputActionValue& InputActionValue);
	void HandleUnCrouch(const FInputActionValue& InputActionValue);
	void HandleSprinting(const FInputActionValue& InputActionValue);
	void HandleStopSprinting(const FInputActionValue& InputActionValue);
	void Shoot(const FInputActionValue& InputActionValue);
	void Reload(const FInputActionValue& InputActionValue);
	void UpdateMovementState();

	UPROPERTY(BlueprintAssignable, Category = "Player Events")
	FPlayerWallRunning OnPlayerWallRunning;

	UPROPERTY(BlueprintAssignable, Category = "Player Events")
	FPlayerWallJumping OnPlayerWallJumping;

	UPROPERTY(BlueprintAssignable, Category = "Player Events")
	FPlayerFalling OnPlayerFalling;

private:
	UPROPERTY(EditAnywhere, Category = "Input Maps")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input Maps")
	TObjectPtr<UInputMappingContext> WeaponsInputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> ReloadAction;
};
