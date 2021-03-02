// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageManager.h"
#include "PackageBase.h"
#include "PackageSpawnActor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"


APackageManager::APackageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APackageManager::GetPackageDetails()
{
	//get data from json file

	//create a package using json data

}

void APackageManager::SpawnPackage(FConfigPackage pds)
{
	FVector* Location(0); //temp location
	APackageBase* spawnedPackage = (APackageBase*)GetWorld()->SpawnActor(APackageBase::StaticClass(), Location);
	//APackageBase* spawnedPackage = (APackageBase*)GetWorld()->SpawnActor(APackageBase::StaticClass(), NAME_None, &Location);
	spawnedPackage->InitialisePackage(pds);
}

void APackageManager::GetSpawnLocations()
{
	//loop through all spawn locations and add to TArray of spawn locations
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APackageSpawnActor::StaticClass(), SpawnPackageLocations);
}

// Called when the game starts or when spawned
void APackageManager::BeginPlay()
{
	Super::BeginPlay();
	//json stuff
	FString FileName(TEXT("Config.json"));
	FString Result;
	auto test = FPaths::ProjectConfigDir();
	FFileHelper::LoadFileToString(Result, *(FPaths::ProjectConfigDir() + FileName));
	FConfig config = FConfig(Result);
	//find all spawn locations and store into a TArray
	GetSpawnLocations();

	// Temp list of spawnpoints
	auto spawnPointsCopy = SpawnPackageLocations;

	for (int i = 0; i < TotalPackagesAmount; ++i) {
		// Get random Spawnpoint
		auto randomNumber = FMath::RandRange(0, spawnPointsCopy.Num() - 1);
		auto spawnPoint = spawnPointsCopy[randomNumber];

		// Remove the spawnpoint from the temporary spawnpoints so we dont spawn at the same place twice 
		spawnPointsCopy.Remove(spawnPoint);

		// Get Random package type
		auto jsonLength = config.packages.Num();
		auto randomNumber2 = FMath::RandRange(0, jsonLength - 1);

		auto packageType = config.packages[randomNumber2];

		// Spawn Package
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		APackageBase* package = GetWorld()->SpawnActor<APackageBase>(spawnPoint->GetActorLocation(), Rotation, SpawnInfo);
		package->InitialisePackage(packageType);

	}
}

// Called every frame
void APackageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
