// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageManager.h"
#include "PackageBase.h"
// Sets default values
APackageManager::APackageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APackageManager::SpawnPackage(FPackageDataStructure pds)
{
	FVector* Location(0); //temp location
	APackageBase* spawnedPackage = (APackageBase*)GetWorld()->SpawnActor(APackageBase::StaticClass(),Location);
	//APackageBase* spawnedPackage = (APackageBase*)GetWorld()->SpawnActor(APackageBase::StaticClass(), NAME_None, &Location);
	spawnedPackage->InitialisePackage(pds);
}

// Called when the game starts or when spawned
void APackageManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APackageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

