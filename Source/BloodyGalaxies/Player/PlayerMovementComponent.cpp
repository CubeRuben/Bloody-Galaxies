#include "PlayerMovementComponent.h"

#include "PlayerPawn.h"

#include "PlayerStatsComponent.h"

#include "../CustomGameUserSettings.h"

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

UPlayerMovementComponent::UPlayerMovementComponent() 
{
	static ConstructorHelpers::FObjectFinder<USoundCue> StepSoundAsset(TEXT("/Game/Audio/Player/SC_MetalFootstep.SC_MetalFootstep"));
	StepSound = StepSoundAsset.Object;

	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMovementComponent::BeginPlay() 
{
	Super::BeginPlay();

	

	PawnOwner = Cast<APlayerPawn>(GetOwner());

	if (!PawnOwner) 
	{
		DestroyComponent();
	}
}

void UPlayerMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) 
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	FHitResult hit;

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredComponent(PawnOwner->GetMovementCollider());

	if (GetWorld()->SweepSingleByProfile(hit, PawnOwner->GetMovementCollider()->GetComponentLocation(), PawnOwner->GetMovementCollider()->GetComponentLocation() + FVector(0, 0, -1), PawnOwner->GetMovementCollider()->GetComponentRotation().Quaternion(), "BlockAll", PawnOwner->GetMovementCollider()->GetCollisionShape(), collisionParams))
	{
		if (PawnOwner->GetPlayerInput()->bJump) 
		{
			PawnOwner->GetMovementCollider()->AddImpulse(FVector(0, 0, 1) * PawnOwner->GetMovementCollider()->GetMass() * 400);

			PawnOwner->GetPlayerInput()->bJump = false;
		}
		else 
		{
			FVector projection = PawnOwner->GetMovementCollider()->GetComponentVelocity();
			projection.Z = 0;

			if (projection.SizeSquared() < FMath::Pow(PawnOwner->GetPlayerStats()->GetMaxSpeed(), 2))
			{
				FVector inputVector = FVector(PawnOwner->GetPlayerInput()->MoveForward, PawnOwner->GetPlayerInput()->MoveRight, 0).RotateAngleAxis(PawnOwner->GetHeadComponent()->GetRelativeRotation().Yaw, FVector(0, 0, 1));
				inputVector.Normalize();

				PawnOwner->GetMovementCollider()->AddForce(inputVector * PawnOwner->GetMovementCollider()->GetMass() * PawnOwner->GetPlayerStats()->GetAcceleration());
			}
		}
	}
	
	UCustomGameUserSettings* gameSettings = Cast<UCustomGameUserSettings>(UCustomGameUserSettings::GetGameUserSettings());
	PawnOwner->GetHeadComponent()->AddLocalRotation(FRotator(0, PawnOwner->GetPlayerInput()->MouseX * gameSettings->GetMouseSensitivity(), 0));
	PawnOwner->GetCameraComponent()->SetRelativeRotation(FRotator(FMath::Clamp(PawnOwner->GetPlayerInput()->MouseY * gameSettings->GetMouseSensitivity() + PawnOwner->GetCameraComponent()->GetRelativeRotation().Pitch, -85, 85), 0, 0));
}
