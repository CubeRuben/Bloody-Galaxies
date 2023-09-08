#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Damageable.h"
#include "PlayerPawn.generated.h"

struct FPlayerInput 
{
	float MoveForward;
	float MoveRight;
	bool bJump;
	bool bShoot;

	float MouseX;
	float MouseY;

	FVector TempTouchLocation;
	bool bTouchPressed;
	float TouchTimer;
	ETouchIndex::Type TouchIndex;
};

UCLASS()
class BLOODYGALAXIES_API APlayerPawn : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:

	APlayerPawn();

protected:

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* MovementCollider;

	UPROPERTY(EditAnywhere)
	class USceneComponent* HeadPoint;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	class UPlayerMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	class UWeaponryComponent* WeaponryComponent;

	UPROPERTY(EditAnywhere)
	class UPlayerStatsComponent* PlayerStatsComponent;

	UPROPERTY(EditAnywhere)
	class UTouchInterface* TouchInterface;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* StepSound;

	FPlayerInput PlayerInput;

	virtual void BeginPlay() override;

	void Destroyed() override;

public:	

	class UCapsuleComponent* GetMovementCollider() { return MovementCollider; }
	class USceneComponent* GetHeadComponent() { return HeadPoint; }
	class UCameraComponent* GetCameraComponent() { return CameraComponent; }
	UFUNCTION(BlueprintCallable)
	class UPlayerStatsComponent* GetPlayerStats() { return PlayerStatsComponent; }
	class UWeaponryComponent* GetWeaponryComponent() { return WeaponryComponent; }

	FPlayerInput* GetPlayerInput() { return &PlayerInput; }
	 
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float damage) override;

	void JumpPressed() { PlayerInput.bJump = true; }
	void JumpReleased() { PlayerInput.bJump = false; }

	void ShootPressed() { PlayerInput.bShoot = true; }
	void ShootReleased() { PlayerInput.bShoot = false; }

	void MoveForwardAxis(float scale) { PlayerInput.MoveForward = scale; }
	void MoveRightAxis(float scale) { PlayerInput.MoveRight = scale; }

	void MouseXAxis(float scale) { PlayerInput.MouseX = scale; }
	void MouseYAxis(float scale) { PlayerInput.MouseY = scale; }

	void TouchPressed(ETouchIndex::Type fingerIndex, FVector location);
	void TouchReleased(ETouchIndex::Type fingerIndex, FVector location);
};
