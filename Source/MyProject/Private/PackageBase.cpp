// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"

// Sets default values
APackageBase::APackageBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //package does not need to tick?
	PackageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	RootComponent = PackageMesh;
}

void APackageBase::InitialisePackage(FConfigPackage pi)
{
	//assign static mesh from data structure
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *pi.ModelReference));
	PackageMesh->SetStaticMesh(mesh);
	//assign random value from data structure value range
	PackageValue = FMath::RandRange(pi.ValueRange[0], pi.ValueRange[1]);
	//assign weight
	PackageMesh->SetMassScale(NAME_None, pi.PackageWeight);
	PackageMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void APackageBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
