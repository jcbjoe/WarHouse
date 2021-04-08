// Fill out your copyright notice in the Description page of Project Settings.


#include "Shutter.h"

// Sets default values
AShutter::AShutter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> redMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerRed.PlayerRed"));
	static ConstructorHelpers::FObjectFinder<UMaterial> blueMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerBlue.PlayerBlue"));
	static ConstructorHelpers::FObjectFinder<UMaterial> yellowMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerYellow.PlayerYellow"));
	static ConstructorHelpers::FObjectFinder<UMaterial> whiteMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerWhite.PlayerWhite"));

	red = redMat.Object;
	blue = blueMat.Object;
	yellow = yellowMat.Object;
	white = whiteMat.Object;

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

void AShutter::SetColour(EPlayerColours colour)
{
	switch (colour)
	{
	case EPlayerColours::White:
		mesh->SetMaterial(0, white);
		break;
	case EPlayerColours::Red:
		mesh->SetMaterial(0, red);
		break;
	case EPlayerColours::Blue:
		mesh->SetMaterial(0, blue);
		break;
	case EPlayerColours::Yellow:
		mesh->SetMaterial(0, yellow);
		break;
	}
}