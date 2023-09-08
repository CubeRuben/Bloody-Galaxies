#include "Door.h"

#include "../Player/PlayerPawn.h"

#include <Components/BoxComponent.h>


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger");
	TriggerBox->SetupAttachment(RootComponent);
	//TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADoor::BeginPlay() 
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnEndOverlap);
}

void ADoor::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (Cast<APlayerPawn>(otherActor)) 
	{
		OpenDoor();
	}
}

void ADoor::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) 
{
	if (Cast<APlayerPawn>(otherActor))
	{
		CloseDoor();
	}
}
