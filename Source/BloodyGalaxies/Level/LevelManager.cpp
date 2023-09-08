#include "LevelManager.h"

#include "LevelPatternGenerator.h"

#include "Room.h"
#include "Door.h"
#include "Corridor.h"
#include "Booster.h"

ALevelManager* ALevelManager::Singleton = nullptr;

ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = true;

	IsLevelCleared = false;
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	Singleton = this;
}

void ALevelManager::BeginDestroy() 
{
	Super::BeginDestroy();

	Singleton = nullptr;
}

void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLevelCleared) 
	{
		CheckLevelClear();
	}
}

void ALevelManager::CheckLevelClear() 
{
	for (ARoom* room : Rooms) 
	{
		if (room->GetRoomStatus() != ERoomStatus::Cleared) 
		{
			return;
		}
	}
	
	IsLevelCleared = true;

	for (ARoom* room : Rooms)
	{
		GetWorld()->SpawnActor<AActor>(LevelEnd, room->GetActorLocation(), FRotator(0));
	}
}

ARoom* ALevelManager::Generate(int seed) 
{
	FLevelPatternGenerator pattern(5, 5);

	FLevelData levelData = pattern.Generate(seed, 0.6);

	for (auto room : levelData.RoomsData) 
	{
		Rooms.Add(GetWorld()->SpawnActor<ARoom>(RoomsClasses[room.Type], GetActorLocation() + room.Offset * RoomSize + CorridorSize * room.Offset, room.Rotation));

		for (auto door : room.DoorsData) 
		{
			ADoor* newDoor = GetWorld()->SpawnActor<ADoor>(DoorClass, GetActorLocation() + room.Offset * RoomSize + CorridorSize * room.Offset + door.Offset * RoomSize, door.Rotation);
			Doors.Add(newDoor);
			Rooms.Last()->AddRoomDoor(newDoor);
		}
	}

	for (auto el : levelData.CorridorsData)
	{
		Corridors.Add(GetWorld()->SpawnActor<ACorridor>(CorridorClass, GetActorLocation() + el.Offset * RoomSize + CorridorSize * el.Offset, el.Rotation));
	}

	ARoom* startRoom = Rooms[FMath::Rand() % Rooms.Num()];
	startRoom->SetAsStartRoom();

	for (ARoom* room : Rooms) 
	{
		room->GenerateSubRooms();
	}

	return startRoom;
}

void ALevelManager::SpawnRandomBooster(FVector location)
{
	if (BoostersClasses.Num() == 0) 
	{
		return;
	}

	GetWorld()->SpawnActor<ABooster>(BoostersClasses[FMath::Rand() % BoostersClasses.Num()].Get(), location, FRotator(0));
}