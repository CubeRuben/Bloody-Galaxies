#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class BLOODYGALAXIES_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemySpawner();

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyPawn> EnemyClass;

	UPROPERTY(EditAnywhere)
	int WaveNumber;

	virtual void BeginPlay() override;

public:	

	void SpawnEnemy();

};
