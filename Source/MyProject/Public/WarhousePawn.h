// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "WarehousePackage.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Particles/ParticleSystem.h"
#include "WarhousePawn.generated.h"

class USpringArmComponent;
UCLASS(Blueprintable)
class AWarhousePawn : public APawn
{
	GENERATED_BODY()

		/* The mesh component */
		UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShipMeshComponent;


public:
	AWarhousePawn();

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

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

	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* HeldLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UParticleSystemComponent* beamEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
		UWidgetComponent* progressBar;




private:
	virtual void BeginPlay() override;

	float _batteryCharge = 100;

	ACameraActor* cam;

	bool isDead = false;

	float MovingBatteryDrain = 5.0;
	float NonMovingBatteryDrain = 2;
	float HoldingBatteryDrain = -5.0;

	float respawnSeconds = 5;
	float respawnCounter = 0;

};