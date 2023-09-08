#include "DeathPawn.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

ADeathPawn::ADeathPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = 600;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-30, 0, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ADeathPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootComponent->AddLocalRotation(FRotator(0, 30 * DeltaTime, 0));
}