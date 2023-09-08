#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Level/BoosterEnums.h"
#include "PlayerHUD.generated.h"


UCLASS()
class BLOODYGALAXIES_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	APlayerHUD();

protected:

	TSubclassOf<UUserWidget> PlayerWidgetClass;
	class UPlayerUI* PlayerWidget;

	TSubclassOf<UUserWidget> DeathWidgetClass;
	class UUserWidget* DeathWidget;

	void BeginPlay() override;

public:

	void BindRooms(TArray<class ARoom*> rooms);

	void Tick(float DeltaTime) override;

	void AddBooster(EBoosterType type);

	UFUNCTION()
	void OnPlayerEnterRoom(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnPlayerLeaveRoom(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	void OnPlayerDeath();
};