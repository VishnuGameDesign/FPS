// Copyright by Vishnu Suresh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/IPickups.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class AFPSPlayer;

UCLASS(Abstract)
class FPS_API AWeaponBase : public AActor, public IIPickups
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void Pickup() override;
	virtual void Drop() override;

	UFUNCTION()
	void HandlePickup();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UBoxComponent> BoxTrigger;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPSPlayer> PlayerRef;
};
