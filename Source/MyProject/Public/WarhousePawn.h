// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "WarhousePawn.generated.h"

UCLASS(Blueprintable)
class AWarhousePawn : public APawn
{
	GENERATED_BODY()


public:
	AWarhousePawn();

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName PickupBinding;

	static const FName ArmForwardBinding;
	static const FName ArmRightBinding;

	void SetIsOnCharger(bool isOnCharger);

	bool IsDead();

protected:
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* BeamMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* HeldLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UParticleSystemComponent* beamEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
		UWidgetComponent* progressBar;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

private:
	virtual void BeginPlay() override;

	float _batteryCharge = 100;

	bool isDead = false;

	const float MovingBatteryDrain = 5.0;
	const float NonMovingBatteryDrain = 2;
	const float SingleHoldingBatteryDrain = -5.0;
	const float MultiHoldingBatteryDrain = -10.0;
	const float chargingPadRate = 20.0f;

	const float respawnSeconds = 5;
	float respawnCounter = 0;

	bool isOnChargingPad = false;

	bool isDownPressed = false;
	bool isUpPressed = false;

	void DownPressed();

	void DownReleased();

	void UpPressed();

	void UpReleased();

};