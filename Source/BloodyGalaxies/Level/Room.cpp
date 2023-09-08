#include "Room.h"

#include "Components/BoxComponent.h"

#include "Door.h"
#include "SubRoom.h"
#include "EnemySpawner.h"
#include "../Player/PlayerPawn.h"
#include "../Enemies/EnemyPawn.h"

FWave::FWave() 
{
	Wave = TArray<AEnemySpawner*>();
}

ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	RoomEnterTrigger = CreateDefaultSubobject<UBoxComponent>("Room Enter Trigger");
	RoomEnterTrigger->InitBoxExtent(FVector(1400, 1400, 400));
	RoomEnterTrigger->SetupAttachment(RootComponent);

	RoomCoollider = CreateDefaultSubobject<UBoxComponent>("Room Coollider");
	RoomCoollider->InitBoxExtent(FVector(1500, 1500, 600));
	RoomCoollider->SetupAttachment(RootComponent);
	RoomCoollider->AddLocalOffset(FVector(0, 0, 600));

	CurrentWave = 0;

	EnemyWaves = TArray<FWave>();
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
	RoomEnterTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnRoomEnter);
}

void ARoom::GenerateSubRooms() 
{
	if (SubRoomClasses.Num() > 0 && RoomSpec == ERoomSpec::Default)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		ASubRoom* subRoom = GetWorld()->SpawnActor<ASubRoom>(SubRoomClasses[FMath::Rand() % SubRoomClasses.Num()], GetActorLocation(), GetActorRotation(), spawnParams);

		subRoom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ARoom::OnRoomEnter(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (!Cast<APlayerPawn>(otherActor)) 
	{
		return;
	}

	if (RoomStatus == ERoomStatus::Cleared) 
	{
		return;
	}

	if (EnemyWaves.Num() == 0)
	{
		RoomStatus = ERoomStatus::Cleared;
		return;
	}

	RoomStatus = ERoomStatus::Waves;

	LockDoors();

	GetWorldTimerManager().SetTimer(EnemyWavesTimer, this, &ARoom::TrySpawnNextWave, 1, true, 1);
}

void ARoom::AddEnemySpawner(int waveNumber, AEnemySpawner* enemySpawner)
{
	if (EnemyWaves.Num() <= waveNumber)
	{
		EnemyWaves.SetNum(waveNumber + 1, false);
	} 

	EnemyWaves[waveNumber].Wave.Add(enemySpawner);
}

void ARoom::TrySpawnNextWave() 
{
	TArray<AActor*> enemies;

	RoomCoollider->GetOverlappingActors(enemies, AEnemyPawn::StaticClass());

	if (enemies.Num() == 0) 
	{
		SpawnNextWave();
	}
}

void ARoom::SpawnNextWave() 
{
	if (EnemyWaves.Num() == CurrentWave) 
	{
		GetWorldTimerManager().ClearTimer(EnemyWavesTimer);

		UnlockDoors();

		RoomStatus = ERoomStatus::Cleared;

		return;
	}

	for (AEnemySpawner* spawner : EnemyWaves[CurrentWave].Wave) 
	{
		spawner->SpawnEnemy();
	}

	CurrentWave++;
}

void ARoom::LockDoors() 
{
	for (ADoor* door : Doors) 
	{
		door->LockDoor();
	}
}

void ARoom::UnlockDoors() 
{
	for (ADoor* door : Doors)
	{
		door->UnlockDoor();
	}
}