// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PackageDataStruct.h"
#include "PackageManager.generated.h"

UCLASS()
class MYPROJECT_API APackageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageManager();
	//variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int TotalPackagesAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnLocations")
		TArray<AActor*> SpawnPackageLocations;
	//functions
	FString GetPackageDetails();
	void SpawnPackage(FConfigPackage pi);
	void GetSpawnLocations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FConfigPackage PackageInfo;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
