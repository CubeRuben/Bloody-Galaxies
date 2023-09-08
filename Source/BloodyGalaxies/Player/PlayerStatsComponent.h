#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Level/BoosterEnums.h"
#include "PlayerStatsComponent.generated.h"


struct BLOODYGALAXIES_API FBoosterEffect
{
	FBoosterEffect(EBoosterType type, float duration);

	float Duration;
	float StartDuration;

	EBoosterType BoosterType;

	float GetProgress() { return Duration / StartDuration; }

	bool Update(float deltaTime);

	void AddDuration(float duration);
};

UCLASS(BlueprintType)
class BLOODYGALAXIES_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStatsComponent();

protected:

	virtual void BeginPlay() override;

	float MaxSpeed;
	float Acceleration;

	float MaxHP;
	float CurrentHP;

	float PassiveRegeneration;
	float PassiveRegenerationCooldownTimerAfterDamage;

	float XP;
	float Level;

	class USoundCue* PainSound;

	TArray<FBoosterEffect> Boosters;

public:	

	float GetMaxSpeed() { return MaxSpeed + MaxSpeed * GetIfHaveBooster(EBoosterType::Accelerator) * 1; }
	float GetAcceleration() { return Acceleration + Acceleration * GetIfHaveBooster(EBoosterType::Accelerator) * 1.5; }

	float GetCurrentHP() { return CurrentHP; }
	float GetMaxHP() { return MaxHP; }

	UFUNCTION(BlueprintCallable)
	bool GetIfHaveBooster(EBoosterType type);

	UFUNCTION(BlueprintCallable)
	float GetBoosterProgress(EBoosterType type);

	float GetXPForNextLevel() { return Level * 550; }

	float GetLevel() { return Level; }
	float GetXP() { return XP; }

	void AddXP(float xp);

	float GetPassiveRegeneration() { return PassiveRegeneration; }

	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	void ApplyDamage(float damage);
	void Heal(float amount);

	void AddBooster(EBoosterType type, float duration);

	UFUNCTION(BlueprintCallable)
	void SaveStats();
};
