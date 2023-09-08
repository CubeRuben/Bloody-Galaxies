#include "BloodyGalaxiesGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Player/PlayerPawn.h"
#include "Player/PlayerHUD.h"

#include "Level/LevelManager.h"
#include "Level/Room.h"


ABloodyGalaxiesGameModeBase::ABloodyGalaxiesGameModeBase() 
{
	DefaultPawnClass = nullptr;
	HUDClass = APlayerHUD::StaticClass();
}

void ABloodyGalaxiesGameModeBase::StartPlay() 
{
	Super::StartPlay();

	LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelManager::StaticClass()));

	if (!LevelManager)
	{
		return;
	}

	ARoom* startRoom = LevelManager->Generate(time(0));

	APlayerHUD* playerHud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	playerHud->BindRooms(LevelManager->GetRooms());

	APawn* player = GetWorld()->SpawnActor<APawn>(APlayerPawn::StaticClass(), startRoom->GetActorLocation() + FVector(0, 0, 200), FRotator(0));
	GetWorld()->GetFirstPlayerController()->Possess(player);
}