#include "PlayerPawn.h"

#include "PlayerMovementComponent.h"
#include "WeaponryComponent.h"
#include "PlayerStatsComponent.h"
#include "../CustomGameUserSettings.h"

#include <Components/CapsuleComponent.h>
#include <Components/SceneComponent.h>
#include <Camera/CameraComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <GameFramework/TouchInterface.h>

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UTouchInterface> TouchInterfaceAsset(TEXT("/Game/TSS_Default.TSS_Default"));

	if (TouchInterfaceAsset.Object)
	{
		TouchInterface = TouchInterfaceAsset.Object;
	}


	MovementCollider = CreateDefaultSubobject<UCapsuleComponent>("Movement Collider");
	MovementCollider->InitCapsuleSize(50, 100);
	MovementCollider->SetCollisionProfileName("BlockAll");
	MovementCollider->BodyInstance.bLockXRotation = true;
	MovementCollider->BodyInstance.bLockYRotation = true;
	MovementCollider->BodyInstance.bLockZRotation = true;
	MovementCollider->SetSimulatePhysics(true);
	



	RootComponent = MovementCollider;

	HeadPoint = CreateDefaultSubobject<USceneComponent>("Head");
	HeadPoint->SetupAttachment(MovementCollider);
	HeadPoint->SetRelativeLocation(FVector(0, 0, 85));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(HeadPoint);

	MovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>("Movement Component");
	WeaponryComponent = CreateDefaultSubobject<UWeaponryComponent>("Weaponry Component");
	PlayerStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>("Stats Component");
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UPhysicalMaterial* physMaterial = NewObject<UPhysicalMaterial>();
	physMaterial->Restitution = 0;
	physMaterial->bOverrideRestitutionCombineMode = true;
	physMaterial->RestitutionCombineMode = EFrictionCombineMode::Min;
	physMaterial->SleepLinearVelocityThreshold = 0;
	physMaterial->Friction = 2;
	MovementCollider->SetPhysMaterialOverride(physMaterial);

	MovementCollider->SetMassOverrideInKg(NAME_None, 100);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

	GetWorld()->GetFirstPlayerController()->ActivateTouchInterface(TouchInterface);

}

void APlayerPawn::Destroyed() 
{
	Super::Destroyed();

	GetWorld()->GetFirstPlayerController()->ActivateTouchInterface(nullptr);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerInput.bTouchPressed) 
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		FVector newLocation;

		if (playerController)
		{
			bool temp;
			playerController->GetInputTouchState(PlayerInput.TouchIndex, newLocation.X, newLocation.Y, temp);
		}

		FVector delta = PlayerInput.TempTouchLocation - newLocation;
		PlayerInput.TempTouchLocation = newLocation;

		PlayerInput.MouseX = -delta.X;
		PlayerInput.MouseY = delta.Y;

		if (!PlayerInput.bShoot)
		{
			PlayerInput.TouchTimer += DeltaTime;

			UCustomGameUserSettings* gameSettings = Cast<UCustomGameUserSettings>(UCustomGameUserSettings::GetGameUserSettings());

			if (gameSettings->GetEnabledHoldToShoot() && PlayerInput.TouchTimer > gameSettings->GetHoldTimeToShoot())
			{
				PlayerInput.bShoot = true;
			}
		}
	}
}

void APlayerPawn::ApplyDamage(float damage)
{ 
	PlayerStatsComponent->ApplyDamage(damage); 
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::JumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerPawn::JumpReleased);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::ShootPressed);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &APlayerPawn::ShootReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForwardAxis);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRightAxis);

	PlayerInputComponent->BindAxis("MouseX", this, &APlayerPawn::MouseXAxis);
	PlayerInputComponent->BindAxis("MouseY", this, &APlayerPawn::MouseYAxis);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerPawn::TouchPressed);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerPawn::TouchReleased);
}

void APlayerPawn::TouchPressed(ETouchIndex::Type fingerIndex, FVector location) 
{
	PlayerInput.bTouchPressed = true;
	
	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (playerController)
	{
		bool temp;
		playerController->GetInputTouchState(fingerIndex, PlayerInput.TempTouchLocation.X, PlayerInput.TempTouchLocation.Y, temp);
	}

	PlayerInput.TouchIndex = fingerIndex;
}

void APlayerPawn::TouchReleased(ETouchIndex::Type fingerIndex, FVector location) 
{
	PlayerInput.bTouchPressed = false;
	PlayerInput.bShoot = false;
	PlayerInput.TouchTimer = 0;
}