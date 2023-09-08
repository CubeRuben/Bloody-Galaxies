#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloodyGalaxiesGameModeBase.generated.h"

UCLASS()
class BLOODYGALAXIES_API ABloodyGalaxiesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ABloodyGalaxiesGameModeBase();

protected:

	UPROPERTY(EditAnywhere)
	class ALevelManager* LevelManager;

	void StartPlay() override;
};
