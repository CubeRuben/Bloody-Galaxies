#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

UCLASS()
class BLOODYGALAXIES_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UCustomGameInstance();

	int Level;
	int XP;

	UFUNCTION(BlueprintCallable)
	void Reset();
};
