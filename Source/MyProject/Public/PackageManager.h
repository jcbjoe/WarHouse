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

	FString GetPackageDetails();
	
	UFUNCTION()
		void SpawnPackage(FConfig config);
	
	UFUNCTION()
		void SpawnSpecialPackages(FConfig config);

	UFUNCTION()
		void SpawnSpecialPackage(FConfig config, FVector location);
	
	void RemovePackage(APackageBase* package);
	
	int GetPackagesLength();
	
	void ActivatePackageTimer();
	
	void NewPackages();

	FVector GetSpawnPosition();
	FConfig GetConfig();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	FConfigPackage PackageInfo;
	FConfig Config;

	UPROPERTY(EditAnywhere, Category = "Data")
		int TotalPackagesAmount = 25;

	UPROPERTY(EditAnywhere)
		float PackageTimer;

private:
	TArray<APackageBase*> Packages;

	//Handle to manage the package timer
	FTimerHandle PackageTimerHandle;
	//Delegate for passing parametrs to function called by timer
	FTimerDelegate PackageTimerDelegate;

	TArray<AActor*> SpawnPackageLocations;

	TArray<AActor*> SpecialPackageLocations;
};
