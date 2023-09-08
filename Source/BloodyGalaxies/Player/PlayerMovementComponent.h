#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"


UCLASS()
class BLOODYGALAXIES_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPlayerMovementComponent();

protected:

	class APlayerPawn* PawnOwner;

	class USoundCue* StepSound;

	FTimerHandle StepSoundHandler;

	void BeginPlay() override;

public:

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;


};
