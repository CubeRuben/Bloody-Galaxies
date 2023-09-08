#include "Bullet.h"

#include "../Damageable.h"

#include <Engine/World.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BarFillObj(TEXT("/Game/Particles/Bullet/NS_BulletTrace.NS_BulletTrace"));
	TracerNiagaraSystem = BarFillObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> RicochetSoundAsset(TEXT("/Game/Audio/Player/SC_Ricochet.SC_Ricochet"));
	RicochetSound = RicochetSoundAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundAsset(TEXT("/Game/Audio/Player/SC_LaserHit.SC_LaserHit"));
	HitSound = HitSoundAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	RicochetCount = 5;
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult hitResult;

	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByProfile(hitResult, GetActorLocation(), GetActorLocation() + Velocity * DeltaTime, "BlockAll", params) )
	{
		SpawnBulletTrace(GetActorLocation(), hitResult.ImpactPoint);

		IDamageableInterface* damageable = Cast<IDamageableInterface>(hitResult.GetActor());

		if (damageable)
		{
			damageable->ApplyDamage(Damage);

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

			Destroy();
		}
		else 
		{
			if (RicochetCount > 0)
			{
				Velocity = FMath::GetReflectionVector(Velocity, hitResult.ImpactNormal);

				SetActorLocation(hitResult.Location + hitResult.ImpactNormal);

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), RicochetSound, GetActorLocation());

				RicochetCount--;
			}
			else 
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

				Destroy();
			}
		}
	}
	else
	{
		SpawnBulletTrace(GetActorLocation(), GetActorLocation() + Velocity * DeltaTime);

		AddActorWorldOffset(Velocity * DeltaTime);
	}
}

void ABullet::InitBullet(FVector velocity, float damage)
{
	Velocity = velocity;
	Damage = damage;
}

void ABullet::SpawnBulletTrace(FVector startPoint, FVector endPoint) 
{
	UNiagaraComponent* tracer = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TracerNiagaraSystem, startPoint);

	tracer->SetVectorParameter("EndPoint", endPoint);
}