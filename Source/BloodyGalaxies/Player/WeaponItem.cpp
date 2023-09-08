#include "WeaponItem.h"

#include "WeaponryComponent.h"
#include "PlayerPawn.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

#include "PlayerStatsComponent.h"
#include "Bullet.h"

AWeaponItem::AWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CooldownTime = 0.25;
	CooldownTimer = 0;
	DamageAmount = 50;
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CooldownTimer > 0) 
	{
		CooldownTimer -= DeltaTime;

		CooldownTimer = FMath::Max(0, CooldownTimer);
	}
}

//
// Firearm
//

AFirearmItem::AFirearmItem() : AWeaponItem()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> LaserShotSoundAsset(TEXT("/Game/Audio/Player/SC_LaserShot.SC_LaserShot"));
	LasetShotSound = LaserShotSoundAsset.Object;

	Overheat = 0;
	OverheatReduce = 0.1;
	OverheatPerShoot = 0.1;
}

void AFirearmItem::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if (playerController)
	{
		APawn* pawn = playerController->GetPawn();

		if (pawn)
		{
			APlayerPawn* playerPawn = Cast<APlayerPawn>(pawn);

			if (playerPawn)
			{
				Overheat = FMath::Max(0, Overheat - (OverheatReduce + OverheatReduce * playerPawn->GetPlayerStats()->GetIfHaveBooster(EBoosterType::Refrigerant) * 1) * DeltaTime);
			}
		}
	}

	

	if (Overheat <= 0)
	{
		Overheated = false;
	}
}

void AFirearmItem::Attack() 
{
	if (CooldownTimer <= 0 && !Overheated)
	{
		FVector shotPoint = OwnerWeaponryComponent->GetPawnOwner()->GetCameraComponent()->GetComponentLocation();
		FVector shotDirection = OwnerWeaponryComponent->GetPawnOwner()->GetCameraComponent()->GetForwardVector();

		Shot(shotPoint, shotDirection * 10000);
	
		if (OwnerWeaponryComponent->GetPawnOwner()->GetPlayerStats()->GetIfHaveBooster(EBoosterType::Prism)) 
		{
			Shot(shotPoint, shotDirection.RotateAngleAxis(15, OwnerWeaponryComponent->GetPawnOwner()->GetCameraComponent()->GetUpVector()) * 10000);
			Shot(shotPoint, shotDirection.RotateAngleAxis(-15, OwnerWeaponryComponent->GetPawnOwner()->GetCameraComponent()->GetUpVector()) * 10000);
		}

		UGameplayStatics::PlaySound2D(GetWorld(), LasetShotSound);

		Overheat += OverheatPerShoot;

		if (Overheat >= 1) 
		{
			Overheated = true;
		}

		CooldownTimer = CooldownTime;
	}
}

void AFirearmItem::Shot(FVector startPoint, FVector velocity)
{
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(startPoint, FRotator(0));

	bullet->InitBullet(velocity, DamageAmount);
	bullet->SetOwner(OwnerWeaponryComponent->GetPawnOwner());

	
}

//
// Melle
//

AMeleeItem::AMeleeItem() 
{

}