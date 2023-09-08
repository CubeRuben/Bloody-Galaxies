#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponItem.generated.h"

UCLASS()
class BLOODYGALAXIES_API AWeaponItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeaponItem();

protected:

	UPROPERTY(EditAnywhere)
	class UWeaponryComponent* OwnerWeaponryComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponMesh;


	UPROPERTY(EditAnywhere)
	float CooldownTime;

	UPROPERTY(EditAnywhere)
	float CooldownTimer;

	UPROPERTY(EditAnywhere)
	float DamageAmount;

	virtual void BeginPlay() override;

public:	

	void SetOwnerComponent(class UWeaponryComponent* weaponryComponent) { OwnerWeaponryComponent = weaponryComponent; }

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() {};

};

UCLASS()
class BLOODYGALAXIES_API AFirearmItem : public AWeaponItem
{
	GENERATED_BODY()

public:

	AFirearmItem();

protected:

	UPROPERTY(EditAnywhere)
	float Overheat;

	UPROPERTY(EditAnywhere)
	float OverheatReduce;

	UPROPERTY(EditAnywhere)
	float OverheatPerShoot;

	UPROPERTY(EditAnywhere)
	bool Overheated;

	class USoundCue* LasetShotSound;

public:

	UFUNCTION(BlueprintCallable)
	float GetOverheat() { return Overheat; }
	UFUNCTION(BlueprintCallable)
	bool IsOverheated() { return Overheated; }

	void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void Shot(FVector startPoint, FVector velocity);

};

UCLASS()
class BLOODYGALAXIES_API AMeleeItem : public AWeaponItem
{
	GENERATED_BODY()

public:

	AMeleeItem();


};