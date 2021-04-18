// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "TruckPackageManager.generated.h"

UCLASS()
class MYPROJECT_API ATruckPackageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATruckPackageManager();

	void IncrementTruckStage(int truckNumber);

	void UpdateTrucks();

	void SetPackagesHidden();

	void CheckIfFull();

	void EmptyTruck(int truckNumber);

	void OpenShutterTruck1();

	void OpenShutterTruck2();

	void OpenShutterTruck3();

	void OpenShutterTruck4();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AStaticMeshActor*> Truck1PackageStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AStaticMeshActor*> Truck2PackageStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AStaticMeshActor*> Truck3PackageStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AStaticMeshActor*> Truck4PackageStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int secondsTillShutterOpens = 3;

private:
	int truck1Stage = 0;
	int truck2Stage = 0;
	int truck3Stage = 0;
	int truck4Stage = 0;

	
	FTimerHandle truck1Handle;
	FTimerHandle truck2Handle;
	FTimerHandle truck3Handle;
	FTimerHandle truck4Handle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
