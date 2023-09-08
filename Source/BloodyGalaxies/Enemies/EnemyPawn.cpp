#include "EnemyPawn.h"

#include "../Level/Booster.h"

#include "../Level/LevelManager.h"

#include "../Player/PlayerPawn.h"
#include "../Player/PlayerStatsComponent.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	StartHP = 100;
	HP = StartHP;

	BoosterSpawnOffset = FVector(0, 0, 100);
	BoosterSpawnChance = 1;
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyPawn::ApplyDamage(float amount)
{
	HP -= amount;

	if (HP <= 0)
	{
		if (FMath::SRand() < BoosterSpawnChance)
		{
			if (ALevelManager::GetSingleton())
			{
				ALevelManager::GetSingleton()->SpawnRandomBooster(GetActorLocation() + BoosterSpawnOffset);
			}
		}

		APlayerController* playerController = GetWorld()->GetFirstPlayerController();

		if (playerController) 
		{
			APawn* pawn = playerController->GetPawn();

			if (pawn) 
			{
				APlayerPawn* playerPawn = Cast<APlayerPawn>(pawn);

				if (playerPawn) 
				{
					playerPawn->GetPlayerStats()->AddXP(StartHP);
				}
			}
		}

		Destroy();
	}
}

