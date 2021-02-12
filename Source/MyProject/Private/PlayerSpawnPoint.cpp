// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpawnPoint.h"

// Sets default values
APlayerSpawnPoint::APlayerSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const auto subObject = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerSpawnPoint"));

	RootComponent = subObject;

	static ConstructorHelpers::FObjectFinder<UMaterial> TransparentMat(TEXT("Material'/Game/Shared/Materials/Transparent.Transparent'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"));
	// Create the mesh component
	ship = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ship->SetStaticMesh(ShipMesh.Object);

	ship->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ship->SetMaterial(0, TransparentMat.Object);
}

// Called when the game starts or when spawned
void APlayerSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	ship->SetHiddenInGame(true);
	ship->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetActorEnableCollision(false);
}

// Called every frame
void APlayerSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

