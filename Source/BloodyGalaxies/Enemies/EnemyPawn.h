#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Damageable.h"

#include "EnemyPawn.generated.h"

UCLASS()
class BLOODYGALAXIES_API AEnemyPawn : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:

	AEnemyPawn();

protected:

	UPROPERTY(EditAnywhere)
	float StartHP;

	UPROPERTY(EditAnywhere)
	float HP;

	UPROPERTY(EditAnywhere)
	FVector BoosterSpawnOffset;

	UPROPERTY(EditAnywhere)
	float BoosterSpawnChance;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void ApplyDamage(float amount) override;

};
