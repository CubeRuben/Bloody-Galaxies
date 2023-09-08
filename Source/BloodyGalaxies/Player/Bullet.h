#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"



UCLASS()
class BLOODYGALAXIES_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	

	ABullet();

protected:

	UPROPERTY(EditAnywhere)
	FVector Velocity;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	int RicochetCount;

	class UParticleSystemComponent* ParticleSystem;

	class UNiagaraSystem* TracerNiagaraSystem;

	class USoundCue* RicochetSound;
	class USoundCue* HitSound;

	void SpawnBulletTrace(FVector startPoint, FVector endPoint);

public:	

	virtual void Tick(float DeltaTime) override;

	void InitBullet(FVector velocity, float damage);
};
