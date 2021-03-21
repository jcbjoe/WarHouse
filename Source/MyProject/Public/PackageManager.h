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

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int TotalPackagesAmount = 1;

	FString GetPackageDetails();
	void SpawnPackage(FConfig config, TArray<AActor*> SpawnPackageLocations);
	void GetSpawnLocations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FConfigPackage PackageInfo;

	TArray<AActor*> SpawnPackageLocations;


};
