// Fill out your copyright notice in the Description page of Project Settings.


#include "Shutter.h"

// Sets default values
AShutter::AShutter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> shutterOpenMesh(TEXT("/Game/Assets/ConorAssets/Shutters/ShuttersOpen.ShuttersOpen"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> shutterClosedMesh(TEXT("/Game/Assets/ConorAssets/Shutters/ShuttersClosed.ShuttersClosed"));

	shutterOpen = shutterOpenMesh.Object;
	shutterClosed = shutterClosedMesh.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));

	mesh->SetStaticMesh(shutterClosed);

	RootComponent = mesh;

}

// Called when the game starts or when spawned
void AShutter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShutter::Open()
{
	mesh->SetStaticMesh(shutterOpen);
}

void AShutter::Close()
{
	mesh->SetStaticMesh(shutterClosed);
}