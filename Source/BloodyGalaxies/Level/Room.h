#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomEnums.h"

#include "Room.generated.h"

USTRUCT()
struct BLOODYGALAXIES_API FWave 
{
	GENERATED_BODY()

public:

	FWave();
	
	UPROPERTY(VisibleAnywhere)
	TArray<class AEnemySpawner*> Wave;
};

UCLASS()
class BLOODYGALAXIES_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	

	ARoom();

protected:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* RoomEnterTrigger;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* RoomCoollider;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERoomType> RoomType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERoomSpec> RoomSpec;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERoomStatus> RoomStatus;

	UPROPERTY(EditAnywhere)
	TArray<class ADoor*> Doors;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ASubRoom>> SubRoomClasses;

	UPROPERTY(VisibleAnywhere)
	TArray<FWave> EnemyWaves;

	UPROPERTY(EditAnywhere)
	int CurrentWave;

	UPROPERTY(EditAnywhere)
	FTimerHandle EnemyWavesTimer;

	UFUNCTION()
	void OnRoomEnter(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual void BeginPlay() override;

public:	

	TEnumAsByte<ERoomStatus> GetRoomStatus() { return RoomStatus; }

	class UBoxComponent* GetRoomCoollider() { return RoomCoollider; };
	
	void SetAsStartRoom() { RoomSpec = ERoomSpec::StartRoom; RoomStatus = ERoomStatus::Cleared; }

	void GenerateSubRooms();

	TArray<class ADoor*> GetDoors() { return Doors; }

	void AddRoomDoor(class ADoor* door) { Doors.Add(door); }
	void AddEnemySpawner(int waveNumber, class AEnemySpawner* enemySpawner);

	void LockDoors();
	void UnlockDoors();

	void TrySpawnNextWave();
	void SpawnNextWave();

};
