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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
		UBoxComponent* collisionMesh = nullptr;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnPickupPressed();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* HeldLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UParticleSystemComponent* beamEmitter;
private:
	virtual void BeginPlay() override;

	bool isCollidingPackage = false;
	UPrimitiveComponent* objCollidingWith = nullptr;



};