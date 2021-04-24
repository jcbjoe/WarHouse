// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PackageDataStruct.h"
#include "PackageBase.h"
#include "PackageManager.generated.h"

UCLASS()
class MYPROJECT_API APackageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageManager();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int TotalPackagesAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int PackageThreshold;

	FString GetPackageDetails();
	UFUNCTION()
		void SpawnPackage(FConfig config);
	UFUNCTION()
		void SpawnSpecialPackage(FConfig config);
	void RemovePackage(APackageBase* package);
	int GetPackagesLength();
	void ActivatePackageTimer();
	void NewPackages();

	FVector GetSpawnPosition();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FConfigPackage PackageInfo;
	FConfig Config;

	TArray<AActor*> SpawnPackageLocations;
	TArray<AActor*> SpecialPackageLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<APackageBase*> Packages;
	UPROPERTY(EditAnywhere)
		float PackageTimer;
	//Handle to manage the package timer
	FTimerHandle PackageTimerHandle;
	//Delegate for passing parametrs to function called by timer
	FTimerDelegate PackageTimerDelegate;
};
