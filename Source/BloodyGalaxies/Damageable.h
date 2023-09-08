#pragma once

#include "Damageable.generated.h"

UINTERFACE()
class BLOODYGALAXIES_API UDamageableInterface : public UInterface
{
	GENERATED_BODY()
	
};

class BLOODYGALAXIES_API IDamageableInterface
{
	GENERATED_BODY()

public:

	virtual void ApplyDamage(float amount) {}
};
