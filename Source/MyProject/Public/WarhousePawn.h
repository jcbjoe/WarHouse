// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerColours.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "NiagaraComponent.h"
#include "WarhousePawn.generated.h"


UCLASS(Blueprintable)
class AWarhousePawn : public APawn
{
	GENERATED_BODY()

public:
	AWarhousePawn();
	void SetIsOnCharger(bool isOnCharger);
	void SetColour(EPlayerColours colour);
	void DropHeldItem();
	void KillPlayer();
	bool IsDead();
protected:
	//--- Unreal overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//--- Unreal Visable components
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* ShipMeshComponent;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BeamMeshComponent;
	UPROPERTY(EditDefaultsOnly)
		UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* HeldLocation;
	UPROPERTY(EditDefaultsOnly)
		UNiagaraComponent* beamEmitter;
	UPROPERTY(EditDefaultsOnly)
		UWidgetComponent* progressBar;
	UPROPERTY(EditDefaultsOnly)
		UDecalComponent* floorDecal;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* beamSource;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* beamTarget;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
private:

	//--- Game Variables
	float _batteryCharge = 100;
	bool isDead = false;
	float respawnCounter = 0;
	bool isOnChargingPad = false;
	float MoveSpeed = 1000.0f;

	//--- Adjustable Variables
	const float MovingBatteryDrain = 5.0;
	const float NonMovingBatteryDrain = 2;
	const float SingleHoldingBatteryDrain = -5.0;
	const float MultiHoldingBatteryDrain = -10.0;
	const float chargingPadRate = 20.0f;
	const float maxUpDownVal = 125;

	const float audioStationaryVolume = 0.05f;
	const float audioMovingVolume = 0.1f;
	const float audioChargingVolume = 0.6f;
	const float audioBeamVolume = 0.1f;
	const float deathSoundVolume = 0.5f;

	const int packageHoldDistance = 160;
	const int packageBeamSourceDistance = 50;

	//rumble variables
	const float RumbleFrequency = 0.12f;
	const float RumbleDuration = 0.15f;

	const float respawnSeconds = 5;
	const float DefaultMoveSpeed = 1000.0f;

	const int beamHeightOffset = 112;

	//--- Input names
	const FName MoveForwardBinding = FName("MoveForward");
	const FName MoveRightBinding = FName("MoveRight");
	const FName PickupBinding = FName("PickupDrop");

	const FName ArmForwardBinding = FName("ArmForward");
	const FName ArmRightBinding = FName("ArmRight");

	const FName LeftTrigger = FName("LeftTrigger");
	const FName RightTrigger = FName("RightTrigger");

	//--- Other Variables
	UMaterial* red;
	UMaterial* redGrav;
	UMaterial* blue;
	UMaterial* blueGrav;
	UMaterial* yellow;
	UMaterial* yellowGrav;
	UMaterial* white;
	UMaterial* whiteGrav;
	UMaterial* glasses;
	UMaterial* glassesGrav;
	UMaterial* smiley;
	UMaterial* smileyGrav;
	UMaterial* tux;
	UMaterial* tuxGrav;
	UMaterial* ironman;
	UMaterial* ironmanGrav;

	USoundBase* dieSoundBase;

	UAudioComponent* audioComp;
	UAudioComponent* beamAudioComp;
	UAudioComponent* chargingComp;

	AActor* lastHeldPackage = nullptr;

	UNiagaraSystem* sparksEmitter;
	float volumeMultiplier = 1.0;

	bool canVibrate = true;
};
