#include "PlayerHUD.h"

#include "PlayerUI.h"
#include "PlayerPawn.h"
#include "PlayerStatsComponent.h"
#include "WeaponryComponent.h"
#include "WeaponItem.h"

#include "../Level/Room.h"
#include "../Level/Door.h"

#include <Components/BoxComponent.h>
#include <Camera/CameraComponent.h>

APlayerHUD::APlayerHUD() 
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerWidgetClassAsset(TEXT("/Game/UserWidgets/UW_PlayerUI"));

	if (PlayerWidgetClassAsset.Class)
	{
		PlayerWidgetClass = PlayerWidgetClassAsset.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DeathWidgetClassAsset(TEXT("/Game/UserWidgets/UW_DeathMenu"));

	if (DeathWidgetClassAsset.Class)
	{
		DeathWidgetClass = DeathWidgetClassAsset.Class;
	}
}

void APlayerHUD::BeginPlay() 
{
	Super::BeginPlay();

	PlayerWidget = CreateWidget<UPlayerUI>(GetOwningPlayerController(), PlayerWidgetClass);
	PlayerWidget->AddToViewport();
}

void APlayerHUD::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	if (PlayerWidget)
	{
		APawn* pawn = GetOwningPlayerController()->GetPawn();

		if (!pawn) 
		{
			return;
		}
		
		APlayerPawn* playerPawn = Cast<APlayerPawn>(pawn);

		if (!playerPawn) 
		{
			return;
		}

		PlayerWidget->UpdateHP(playerPawn->GetPlayerStats()->GetCurrentHP(), playerPawn->GetPlayerStats()->GetMaxHP());
		PlayerWidget->UpdatePlayerMarker(playerPawn->GetCameraComponent()->GetComponentRotation().Yaw + 90);
		PlayerWidget->UpdateXPAndLevel(playerPawn->GetPlayerStats()->GetXP() / playerPawn->GetPlayerStats()->GetXPForNextLevel(), playerPawn->GetPlayerStats()->GetLevel());

		AWeaponItem* currentWeapon = playerPawn->GetWeaponryComponent()->GetCurrentWeapon();

		if (currentWeapon)
		{
			AFirearmItem* currentGun = Cast<AFirearmItem>(currentWeapon);

			if (currentGun) 
			{
				PlayerWidget->UpdateOverheat(currentGun->GetOverheat(), currentGun->IsOverheated());
			}
		}
	}


}

void APlayerHUD::BindRooms(TArray<class ARoom*> rooms) 
{
	for (ARoom* room : rooms) 
	{
		room->GetRoomCoollider()->OnComponentBeginOverlap.AddDynamic(this, &APlayerHUD::OnPlayerEnterRoom);
		room->GetRoomCoollider()->OnComponentEndOverlap.AddDynamic(this, &APlayerHUD::OnPlayerLeaveRoom);
	}
}

void APlayerHUD::AddBooster(EBoosterType type)
{
	if (PlayerWidget) 
	{
		PlayerWidget->AddBooster(type);
	}
}

void APlayerHUD::OnPlayerEnterRoom(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) 
{
	if (!Cast<APlayerPawn>(otherActor) || !PlayerWidget)
	{
		return;
	}

	ARoom* room = Cast<ARoom>(overlappedComponent->GetAttachParentActor());

	TArray<int> doors;

	for (ADoor* door : room->GetDoors()) 
	{
		switch (FMath::RoundToInt(door->GetActorRotation().Yaw))
		{
		case 0:
			doors.Add(2);
			break;
		case 90:
			doors.Add(3);
			break;
		case 180:
			doors.Add(0);
			break;
		case -90:
			doors.Add(1);
			break;
		}
	}

	PlayerWidget->AddRoomToMinimap(FMath::RoundToInt(overlappedComponent->GetComponentLocation().X / 4500.f), FMath::RoundToInt(overlappedComponent->GetComponentLocation().Y / 4500.f), doors);
	PlayerWidget->MovePlayerMarkerToRoom(FMath::RoundToInt(overlappedComponent->GetComponentLocation().X / 4500.f), FMath::RoundToInt(overlappedComponent->GetComponentLocation().Y / 4500.f));
}

void APlayerHUD::OnPlayerLeaveRoom(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (!Cast<APlayerPawn>(otherActor) || !PlayerWidget)
	{
		return;
	}


}

void APlayerHUD::OnPlayerDeath() 
{
	if (PlayerWidget)
	{
		PlayerWidget->RemoveFromParent();
	}
	
	DeathWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), DeathWidgetClass);
	DeathWidget->AddToViewport();
}