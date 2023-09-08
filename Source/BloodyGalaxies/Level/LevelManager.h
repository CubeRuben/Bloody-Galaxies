#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomEnums.h"
#include "LevelManager.generated.h"

UCLASS()
class BLOODYGALAXIES_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ALevelManager();

protected:
	
	bool IsLevelCleared;

	static ALevelManager* Singleton;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ERoomType>, TSubclassOf<class ARoom>> RoomsClasses;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADoor> DoorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACorridor> CorridorClass;

	UPROPERTY(EditAnywhere)
	float RoomSize;

	UPROPERTY(EditAnywhere)
	float CorridorSize;

	UPROPERTY()
	TArray<class ARoom*> Rooms;

	UPROPERTY()
	TArray<class ADoor*> Doors;

	UPROPERTY()
	TArray<class ACorridor*> Corridors;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ABooster>> BoostersClasses;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> LevelEnd;

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	

	static ALevelManager* GetSingleton() { return Singleton; };

	TArray<class ARoom*> GetRooms() { return Rooms; }

	ARoom* Generate(int seed);

	void CheckLevelClear();

	virtual void Tick(float DeltaTime) override;

	void SpawnRandomBooster(FVector location);

};
