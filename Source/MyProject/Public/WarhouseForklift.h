// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "PackageManager.h"
#include "PackageBase.h"
#include "Components/AudioComponent.h"
#include "WarhouseForklift.generated.h"

class UCurveFloat;

UCLASS()
class MYPROJECT_API AWarhouseForklift : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseForklift();
	//UFunctions
	UFUNCTION()
		void DeliverPackages();
	UFUNCTION()
		void ResumeMovement();
	UFUNCTION()
		void GetReadyToDeliver();
	UFUNCTION()
		void TimelineProgress(float value);
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//normal functions
	virtual void Tick(float DeltaTime) override;
	void Stop();
	void PrepareForkliftForAnotherDelivery();
	void AddPackageToArray(APackageBase* package);
	void RemoveAndDestroyPackages();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* FrontWheels;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BackWheels;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Blades;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* LiftBit;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Pallet;
	UPROPERTY(EditAnywhere)
		float ForkliftWaitSeconds = 5.0f;
	UPROPERTY(EditAnywhere)
		bool isMoving;
	UPROPERTY(EditAnywhere)
		float Speed = 500.0f;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxComponent;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* PackageSpawn1;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* PackageSpawn2;
	UPROPERTY(EditDefaultsOnly)
		UAudioComponent* AudioComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
		UCurveFloat* CurveFloat;
	UPROPERTY()
		FVector StartLocation = FVector(-200.0f, -4174.0f, 0);
	UPROPERTY()
		FVector EndLocation;
	UPROPERTY()
		FVector Location;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		float ZOffset;
	UPROPERTY(EditAnywhere, Category = "Data")
		FRotator TurnAround;

	//member variables
	float DefaultSpeed = 500.0f;
	float WheelRotation = 0;
	float volumeMultiplier = 0.0f;
	const float engineSoundVolume = 0.5f;
	//array to hold all packages on the forklift
	TArray<APackageBase*> PackagesToRemove;
	//timer handle
	FTimerHandle ForkliftTimerHandle;
	//anim stuff
	FTimeline CurveTimeline;
	//movement functions
	void RotateWheels();
	void MoveForklift(float DeltaTime);
	void RotateForklift();
public:

};
