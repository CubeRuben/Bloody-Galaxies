#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DeathPawn.generated.h"

UCLASS()
class BLOODYGALAXIES_API ADeathPawn : public APawn
{
	GENERATED_BODY()

public:

	ADeathPawn();

protected:

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

public:	

	virtual void Tick(float DeltaTime) override;

};
