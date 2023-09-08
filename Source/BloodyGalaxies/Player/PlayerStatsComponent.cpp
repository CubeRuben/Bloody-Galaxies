#include "PlayerStatsComponent.h"

#include "DeathPawn.h"
#include "PlayerHUD.h"

#include "../CustomGameInstance.h"

#include "Kismet/GameplayStatics.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

FBoosterEffect::FBoosterEffect(EBoosterType type, float duration) 
{
	BoosterType = type;
	Duration = duration;
	StartDuration = duration;
}

bool FBoosterEffect::Update(float deltaTime) 
{
	Duration -= deltaTime;

	return Duration <= 0;
}

void FBoosterEffect::AddDuration(float duration) 
{
	Duration += duration;

	StartDuration = FMath::Max(Duration, StartDuration);
}

UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> PainSoundAsset(TEXT("/Game/Audio/Player/SC_Pain.SC_Pain"));
	PainSound = PainSoundAsset.Object;

	MaxSpeed = 550;
	Acceleration = 2500;

	MaxHP = 100;
	CurrentHP = MaxHP;

	PassiveRegeneration = 5;

	Level = 1;
	XP = 0;
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	UCustomGameInstance* gameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>();

	Level = gameInstance->Level;
	XP = gameInstance->XP;

	MaxHP += 25 * (Level - 1);
	CurrentHP += 25 * (Level - 1);
	MaxSpeed += 15 * (Level - 1);
	Acceleration += 50 * (Level - 1);
}

void UPlayerStatsComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (GetIfHaveBooster(EBoosterType::FirstAid))
	{
		Heal(PassiveRegeneration * 2.5 * deltaTime);
	}
	else if (PassiveRegenerationCooldownTimerAfterDamage > 5)
	{
		Heal(PassiveRegeneration * deltaTime);
	}
	else 
	{
		PassiveRegenerationCooldownTimerAfterDamage += deltaTime;
	}

	for (int i = 0; i < Boosters.Num(); i++) 
	{
		if (Boosters[i].Update(deltaTime)) 
		{
			Boosters.RemoveAt(i);
			i--;
		}
	}
}

void UPlayerStatsComponent::AddXP(float xp) 
{
	XP += xp;

	if (XP > GetXPForNextLevel()) 
	{
		XP -= GetXPForNextLevel();

		Level++;

		MaxHP += 25;
		CurrentHP += 25;
		MaxSpeed += 15;
		Acceleration += 50;
	}
}

void UPlayerStatsComponent::ApplyDamage(float damage)
{
	CurrentHP -= damage;
	PassiveRegenerationCooldownTimerAfterDamage = 0;

	UGameplayStatics::PlaySound2D(GetWorld(), PainSound);

	if (CurrentHP <= 0)
	{
		ADeathPawn* deathPawn = GetWorld()->SpawnActor<ADeathPawn>(GetOwner()->GetActorLocation(), FRotator(0));

		APawn* ownerPawn = Cast<APawn>(GetOwner());

		if (ownerPawn) 
		{
			if (ownerPawn->GetController()) 
			{
				ownerPawn->GetController()->Possess(deathPawn);
			}
		}

		APlayerHUD* hud = GetWorld()->GetFirstPlayerController()->GetHUD<APlayerHUD>();

		if (hud)
		{
			hud->OnPlayerDeath();
		}

		ownerPawn->Destroy();
	}

}

void UPlayerStatsComponent::Heal(float amount) 
{
	CurrentHP += amount;

	CurrentHP = FMath::Min(CurrentHP, MaxHP);
}

bool UPlayerStatsComponent::GetIfHaveBooster(EBoosterType type)
{
	return Boosters.ContainsByPredicate([type](const FBoosterEffect& booster) { return booster.BoosterType == type; });
}

float UPlayerStatsComponent::GetBoosterProgress(EBoosterType type) 
{
	return Boosters[Boosters.IndexOfByPredicate([type](const FBoosterEffect& booster) { return booster.BoosterType == type; })].GetProgress();
}

void UPlayerStatsComponent::AddBooster(EBoosterType type, float duration) 
{
	int index = Boosters.IndexOfByPredicate([type](const FBoosterEffect& booster) { return booster.BoosterType == type; });

	if (index == INDEX_NONE) 
	{
		Boosters.Add(FBoosterEffect(type, duration));

		APlayerHUD* hud = GetWorld()->GetFirstPlayerController()->GetHUD<APlayerHUD>();

		if (hud)
		{
			hud->AddBooster(type);
		}
	}
	else
	{
		Boosters[index].AddDuration(duration);
	}

	
}

void UPlayerStatsComponent::SaveStats()
{
	UCustomGameInstance* gameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>();

	gameInstance->Level = Level;
	gameInstance->XP = XP;
}