// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageManager.h"
#include "PackageSpawnActor.h"
#include "SpecialPackageSpawnActor.h"
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

void APackageManager::SpawnPackage(FConfig config)
{
	FVector spawnLocation = GetSpawnPosition();

	// Get Random package type
	auto jsonLength = config.packages.Num();
	auto randomNumber2 = FMath::RandRange(0, jsonLength - 1);

	auto packageType = config.packages[randomNumber2];

	// Spawn Package
	//FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f); //we may need to change this later to an actual rotation
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APackageBase* package = GetWorld()->SpawnActor<APackageBase>(spawnLocation, Rotation, SpawnInfo);
	package->InitialisePackage(packageType);
	Packages.Add(package);
}

void APackageManager::SpawnSpecialPackage(FConfig config)
{
	for (int i = 0; i < SpecialPackageLocations.Num(); i++)
	{
		//get location to spawn
		FVector Location = SpecialPackageLocations[i]->GetActorLocation();
		// Get Random package type
		auto jsonLength = config.packages.Num();
		auto randomNumber2 = FMath::RandRange(0, jsonLength - 1);
		auto packageType = config.packages[randomNumber2];
		FRotator Rotation(0.0f, 0.0f, 0.0f); //we may need to change this later to an actual rotation
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//spawn package
		APackageBase* package = GetWorld()->SpawnActor<APackageBase>(Location, Rotation, SpawnInfo);
		package->InitialisePackage(packageType);
		//make special
		package->SetPackageSpecial();
		//add to list of packages
		Packages.Add(package);
	}
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
	int PackagesNeeded = TotalPackagesAmount - Packages.Num();
	for (int i = 0; i < PackagesNeeded; ++i)
	{
		SpawnPackage(Config);
	}
}

// Called when the game starts or when spawned
void APackageManager::BeginPlay()
{
	Super::BeginPlay();

	//get json data
	FString Result = GetPackageDetails();

	//pass package data into the config struct
	Config = FConfig(Result);

	//find all spawn locations and store into a TArray
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APackageSpawnActor::StaticClass(), SpawnPackageLocations);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpecialPackageSpawnActor::StaticClass(), SpecialPackageLocations);

	for (int i = 0; i < TotalPackagesAmount; ++i)
	{
		SpawnPackage(Config);
	}

	SpawnSpecialPackage(Config);
}

// Called every frame
void APackageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector APackageManager::GetSpawnPosition()
{
	bool found = false;
	FVector spawnLoc;
	do
	{
		int randomNumber = FMath::RandRange(0, SpawnPackageLocations.Num() - 1);
		AActor* spawnPoint = SpawnPackageLocations[randomNumber];

		TArray<AActor*> packages;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APackageBase::StaticClass(), packages);
		bool tooClose = false;
		for (AActor* package : packages)
		{
			if (FVector::Distance(spawnPoint->GetActorLocation(), package->GetActorLocation()) < 100)
			{
				tooClose = true;
				break;
			}
		}

		if (!tooClose)
		{
			found = true;
			spawnLoc = spawnPoint->GetActorLocation();
		}
	} while (!found);

	return spawnLoc;
}

FConfig APackageManager::GetConfig()
{
	return Config;
}
