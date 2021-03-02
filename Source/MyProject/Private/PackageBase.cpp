// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"

// Sets default values
APackageBase::APackageBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //package does not need to tick?
}

void APackageBase::InitialisePackage(FPackageDataStructure pds)
{
	//assign static mesh from data structure 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> packageMesh(*(pds.ModelReference));
	PackageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	PackageMesh->SetStaticMesh(packageMesh.Object);
	
	//assign random value from data structure value range
	PackageValue = FMath::RandRange(pds.ValueRange[0], pds.ValueRange[1]);

	//assign weight
	PackageMesh->SetMassScale(NAME_None, pds.PackageWeight);
}

// Called when the game starts or when spawned
void APackageBase::BeginPlay()
{
	Super::BeginPlay();
	PackageMesh->SetSimulatePhysics(true);
}

// Called every frame
void APackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

