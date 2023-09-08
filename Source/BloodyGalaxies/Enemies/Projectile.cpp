#include "Projectile.h"

#include <Components/StaticMeshComponent.h>

#include "../Player/PlayerPawn.h"
#include "../Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Root Mesh");
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComponent;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnActorHit);
}

void AProjectile::OnActorHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	APlayerPawn* player = Cast<APlayerPawn>(otherActor);

	if (player)
	{
		IDamageableInterface* damageable = Cast<IDamageableInterface>(player);

		if (damageable) 
		{
			damageable->ApplyDamage(DamageAmount);

			Destroy();
		}
	}

	if (!Cast<AProjectile>(otherActor))
	{
		HitSurface(hit.Normal);

		Destroy();
	}
}