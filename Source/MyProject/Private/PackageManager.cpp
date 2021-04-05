// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageManager.h"
#include "PackageSpawnActor.h"
#include "WarhouseHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"


APackageManager::APackageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FString APackageManager::GetPackageDetails()
{
	//set json file name
	FString FileName(TEXT("Config.json"));
	//where we will store the resulting json
	FString Result;
	//get data from json file
	FFileHelper::LoadFileToString(Result, *(FPaths::ProjectConfigDir() + FileName));

	return Result;
}

void APackageManager::SpawnPackage(FConfig config, TArray<AActor*>& SpawnPackageLocationsCopy)
{
	// Get random Spawnpoint
	auto randomNumber = FMath::RandRange(0, SpawnPackageLocationsCopy.Num() - 1);
	auto spawnPoint = SpawnPackageLocationsCopy[randomNumber];

	// Remove the spawnpoint from the temporary spawnpoints so we dont spawn at the same place twice 
	SpawnPackageLocationsCopy.Remove(spawnPoint);

	// Get Random package type
	auto jsonLength = config.packages.Num();
	auto randomNumber2 = FMath::RandRange(0, jsonLength - 1);

	auto packageType = config.packages[randomNumber2];

	// Spawn Package
	//FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f); //we may need to change this later to an actual rotation
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APackageBase* package = GetWorld()->SpawnActor<APackageBase>(spawnPoint->GetActorLocation(), Rotation, SpawnInfo);
	package->InitialisePackage(packageType);
	Packages.Add(package);
}

void APackageManager::GetSpawnLocations()
{
	//loop through all spawn locations and add to TArray of spawn locations
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APackageSpawnActor::StaticClass(), SpawnPackageLocations);
}

void APackageManager::RemovePackage(APackageBase* package)
{
	Packages.RemoveSingle(package);
}

int APackageManager::GetPackagesLength()
{
	return Packages.Num();
}

void APackageManager::ActivatePackageTimer()
{
	// Temp list of spawnpoints
	auto spawnPointsCopy = SpawnPackageLocations;
	GetWorld()->GetTimerManager().SetTimer(PackageTimerHandle, this, &APackageManager::NewPackages, PackageTimer, false);
}

void APackageManager::NewPackages()
{
	// Temp list of spawnpoints
	auto spawnPointsCopy = SpawnPackageLocations;
	int PackagesNeeded = TotalPackagesAmount - Packages.Num();
	for (int i = 0; i < PackagesNeeded; ++i)
	{
		SpawnPackage(Config, spawnPointsCopy);
	}
}

// Called when the game starts or when spawned
void APackageManager::BeginPlay()
{
	Super::BeginPlay();
	//set package threshhold for spawning new packages
	PackageThreshold = TotalPackagesAmount * 0.5;
	//get json data
	FString Result = GetPackageDetails();
	//pass package data into the config struct
	Config = FConfig(Result);
	//find all spawn locations and store into a TArray
	GetSpawnLocations();

	// Temp list of spawnpoints
	auto spawnPointsCopy = SpawnPackageLocations;

	for (int i = 0; i < TotalPackagesAmount; ++i)
	{
		SpawnPackage(Config, spawnPointsCopy);
	}
}

// Called every frame
void APackageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
