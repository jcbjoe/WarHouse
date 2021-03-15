// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"

// Sets default values
APackageBase::APackageBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //package does not need to tick?
	PackageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	RootComponent = PackageMesh;
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Assets/JoeAssets/Package/Glow.Glow"));
	meshMaterial = FoundMaterial.Object;
}

void APackageBase::InitialisePackage(FConfigPackage pi)
{
	Package.PackageName = pi.PackageName;
	Package.ModelReference = pi.ModelReference;
	Package.ValueRange = pi.ValueRange;
	Package.Rarity = pi.Rarity;
	Package.PackageWeight = pi.PackageWeight;

	//assign static mesh from data structure
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *pi.ModelReference));
	PackageMesh->SetStaticMesh(mesh);
	//assign random value from data structure value range
	PackageValue = FMath::RandRange(pi.ValueRange[0], pi.ValueRange[1]);
	//assign weight
	PackageMesh->SetMassScale(NAME_None, pi.PackageWeight);
	PackageMesh->SetSimulatePhysics(true);
	//assign material
	PackageMesh->SetMaterial(0, meshMaterial);
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
