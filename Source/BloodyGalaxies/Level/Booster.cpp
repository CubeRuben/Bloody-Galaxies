#include "Booster.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

#include "../Player/PlayerPawn.h"
#include "../Player/PlayerStatsComponent.h"

ABooster::ABooster()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> PickupSoundAsset(TEXT("/Game/Audio/Player/SC_BoosterPickUp.SC_BoosterPickUp"));
	PickupSound = PickupSoundAsset.Object;


	PlayerTrigger = CreateDefaultSubobject<USphereComponent>("Root");
	PlayerTrigger->InitSphereRadius(150);
	RootComponent = PlayerTrigger;

	BoosterMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	BoosterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoosterMesh->SetupAttachment(RootComponent);

	SpinSpeed = 45;
	LevitationSpeed = 1.5;
	LevitationOffset = 35;
}

void ABooster::BeginPlay() 
{
	Super::BeginPlay();

	PlayerTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABooster::OnPlayerOverlapp);
}

void ABooster::OnPlayerOverlapp(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) 
{
	APlayerPawn* player = Cast<APlayerPawn>(otherActor);

	if (!player) 
	{
		return;
	}

	player->GetPlayerStats()->AddBooster(BoosterType, Duration);

	UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);

	Destroy();
}

void ABooster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BoosterMesh->AddLocalRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
	BoosterMesh->SetRelativeLocation(FVector(0, 0, FMath::Sin(GetWorld()->GetTimeSeconds() * LevitationSpeed) * LevitationOffset));
}

