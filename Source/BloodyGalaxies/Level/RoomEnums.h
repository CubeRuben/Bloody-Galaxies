#pragma once

#include "CoreMinimal.h"

UENUM()
enum ERoomType
{
	DeadEnd = 0,
	Straight,
	Curve,
	TIntersection,
	XIntersection
};

UENUM()
enum ERoomSpec
{
	Default = 0,
	StartRoom
};

UENUM()
enum ERoomStatus
{
	NotCleared,
	Waves,
	Cleared,
};