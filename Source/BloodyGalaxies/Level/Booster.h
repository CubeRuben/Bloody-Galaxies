#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoosterEnums.h"
#include "Booster.generated.h"

UCLASS()
class BLOODYGALAXIES_API ABooster : public AActor
{
	GENERATED_BODY()
	
public:	

	ABooster();

protected:

	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BoosterMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerTrigger;

	UPROPERTY(EditAnywhere)
	float SpinSpeed;

	UPROPERTY(EditAnywhere)
	float LevitationSpeed;

	UPROPERTY(EditAnywhere)
	float LevitationOffset;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBoosterType> BoosterType;

	UPROPERTY(EditAnywhere)
	float Duration;

	UFUNCTION()
	void OnPlayerOverlapp(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
