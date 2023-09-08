#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class BLOODYGALAXIES_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	

	ADoor();

protected:


	UPROPERTY(EditAnywhere)
	class UBoxComponent* TriggerBox;

	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void CloseDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void LockDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void UnlockDoor();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
