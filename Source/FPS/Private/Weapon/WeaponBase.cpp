// Copyright by Vishnu Suresh


#include "Weapon/WeaponBase.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Character/Player/FPSPlayer.h"
#include "Components/BoxComponent.h"
#include "Controller/FPSPlayerController.h"
#include "Preferences/PersonaOptions.h"
#include "Slate/SGameLayerManager.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	WeaponMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = WeaponMesh;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(WeaponMesh);
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	// binding to overlap event
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnOverlapBegin);
}

void AWeaponBase::Pickup()
{
	check(PlayerRef)
	FName SocketName = PlayerRef->WeaponSocketName;
	WeaponMesh->SetSimulatePhysics(false);
	const TObjectPtr<USkeletalMeshComponent> PlayerMesh = PlayerRef->GetPlayerMesh();

	WeaponMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void AWeaponBase::Drop()
{
}

void AWeaponBase::HandlePickup()
{
	Pickup();
}

void AWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player %p %s"),OtherActor, *OtherActor->GetName());
	
	if (OtherActor && OtherActor != this)
	{
		if (AFPSPlayer* Player = Cast<AFPSPlayer>(OtherActor))
		{
			PlayerRef = Player;
			if (AController* Controller = Cast<AController>(PlayerRef))
			{
				if (AFPSPlayerController* PC = Cast<AFPSPlayerController>(Controller))
				{
					PC->OnPickupPressed.AddDynamic(this, &AWeaponBase::HandlePickup);
				}
			}
		}
	}
}

