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
#include "WarhouseForklift.generated.h"

class UCurveFloat;

UCLASS()
class MYPROJECT_API AWarhouseForklift : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseForklift();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* FrontWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BackWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Blades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* LiftBit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Pallet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		float ForkliftWaitSeconds = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		bool isMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		float Speed = 500.0f;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package Spawn Points")
		UChildActorComponent* PackageSpawn1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package Spawn Points")
		UChildActorComponent* PackageSpawn2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package Manager")
		APackageManager* PackageManager;

	float DefaultSpeed = 500.0f;

	float WheelRotation = 0;

	FTimerHandle ForkliftTimerHandle;
	//anim stuff
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
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
	UPROPERTY(EditAnywhere, Category = "Data")
		TArray<APackageBase*> PackagesToRemove;

	void Stop();
	void RotateWheels();
	void MoveForklift(float DeltaTime);
	void RotateForklift();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PrepareForkliftForAnotherDelivery();
	void AddPackageToArray(APackageBase* package);
	void RemoveAndDestroyPackages();
};
