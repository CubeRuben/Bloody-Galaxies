#include "EnemySpawner.h"

#include "../Enemies/EnemyPawn.h"

#include "SubRoom.h"
#include "Room.h"

#include <Components/SphereComponent.h>

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>("Root");
	RootComponent = sphereComponent;
	sphereComponent->SetCollisionProfileName("NoCollision");
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	ASubRoom* subRoom = Cast<ASubRoom>(GetAttachParentActor());

	if (!subRoom) 
	{
		UE_LOG(LogTemp, Log, TEXT("AEnemySpawner - Owner is not ASubRoom"))
		return;
	}

	ARoom* room = Cast<ARoom>(subRoom->GetOwner());

	if (!room)
	{
		UE_LOG(LogTemp, Log, TEXT("AEnemySpawner - Owner of ASubRoom is not ARoom"))
		return;
	}

	room->AddEnemySpawner(WaveNumber, this);
}


void AEnemySpawner::SpawnEnemy()
{
	FActorSpawnParameters actorSpawnParams;
	actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(EnemyClass, GetActorLocation(), FRotator(0, -90, 0), actorSpawnParams);
}