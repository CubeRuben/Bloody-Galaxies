#include "WeaponryComponent.h"

#include "PlayerPawn.h"
#include "WeaponItem.h"

#include <Camera/CameraComponent.h>

UWeaponryComponent::UWeaponryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponryComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APlayerPawn>(GetOwner());

	if (!PawnOwner)
	{
		DestroyComponent();
	}

	SetWeapon(GetWorld()->SpawnActor<AFirearmItem>());
}

void UWeaponryComponent::BeginDestroy() 
{
	Super::BeginDestroy();

	if (PrimaryWeapon)
	{
		PrimaryWeapon->Destroy();
	}
}

void UWeaponryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PawnOwner->GetPlayerInput()->bShoot) 
	{
		PrimaryWeapon->Attack();
	}
}

void UWeaponryComponent::SetWeapon(AWeaponItem* newWeapon) 
{
	PrimaryWeapon = newWeapon;
	newWeapon->SetOwnerComponent(this);
}