#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLOODYGALAXIES_API UWeaponryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWeaponryComponent();

protected:

	class APlayerPawn* PawnOwner;

	class AWeaponItem* PrimaryWeapon;

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	
	class AWeaponItem* GetCurrentWeapon() { return PrimaryWeapon; }

	class APlayerPawn* GetPawnOwner() { return PawnOwner; };

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeapon(AWeaponItem* newWeapon);
};
