#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Level/BoosterEnums.h"
#include "PlayerUI.generated.h"

UCLASS()
class BLOODYGALAXIES_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:


public:

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHP(float hp, float maxHp);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOverheat(float overheat, bool isOverheated);

	UFUNCTION(BlueprintImplementableEvent)
	void AddRoomToMinimap(int x, int y, const TArray<int>& doors);

	UFUNCTION(BlueprintImplementableEvent)
	void MovePlayerMarkerToRoom(int x, int y);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerMarker(float rotation);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateXPAndLevel(float progress, int level);

	UFUNCTION(BlueprintImplementableEvent)
	void AddBooster(EBoosterType type);
};
