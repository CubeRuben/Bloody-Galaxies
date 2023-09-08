#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBoosterType : uint8
{
	NONE = 0,
	Accelerator		UMETA(DisplayName = "Accelerator"),
	Refrigerant		UMETA(DisplayName = "Refrigerant"),
	FirstAid		UMETA(DisplayName = "FirstAid"),
	Prism			UMETA(DisplayName = "Prism")
};