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

	static ConstructorHelpers::FObjectFinder<UMaterial> glassesMat(TEXT("/Game/Assets/ConorAssets/Player/Glasses/GlassesColour.GlassesColour"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ironManMat(TEXT("/Game/Assets/ConorAssets/Player/IronMan/IronManColour.IronManColour"));
	static ConstructorHelpers::FObjectFinder<UMaterial> smileyMat(TEXT("/Game/Assets/ConorAssets/Player/Smiley/SmileyColour.SmileyColour"));
	static ConstructorHelpers::FObjectFinder<UMaterial> tuxMat(TEXT("/Game/Assets/ConorAssets/Player/Tux/TuxColour.TuxColour"));
	//assign colours of the players to the corresponding shutter
	red = redMat.Object;
	blue = blueMat.Object;
	yellow = yellowMat.Object;
	white = whiteMat.Object;

	glasses = glassesMat.Object;
	ironMan = ironManMat.Object;
	smiley = smileyMat.Object;
	tux = tuxMat.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> shutterMesh(TEXT("/Game/Assets/ConorAssets/Shutters/ShutterDoor_1.ShutterDoor_1"));
	//set up root
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetStaticMesh(shutterMesh.Object);
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AShutter::BeginPlay()
{
	Super::BeginPlay();

	minZ = GetActorLocation().Z;
	maxZ = GetActorLocation().Z + MaxMoveAmount;

}

// Called every frame
void AShutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (opening || closing) {
		float amount = ShutterMovementSpeed * DeltaTime;

		currentMoveAmount += amount;

		FVector pos = GetActorLocation();

		if (opening)
		{
			pos.Z = FMath::Clamp(pos.Z + amount, minZ, maxZ);
			SetActorLocation(pos);
		}

		if (closing)
		{
			pos.Z = FMath::Clamp(pos.Z - amount, minZ, maxZ);
			SetActorLocation(pos);
		}

		if (currentMoveAmount > MaxMoveAmount || GetActorLocation().Z >= maxZ || GetActorLocation().Z <= minZ)
		{
			currentMoveAmount = 0;
			if (opening)
			{
				opening = false;
				isOpen = true;
			}

			if (closing)
			{
				closing = false;
				isOpen = false;
			}

		}
	}
}

void AShutter::Open()
{
	currentMoveAmount = 0;
	opening = true;
	closing = false;
}

void AShutter::Close()
{
	currentMoveAmount = 0;
	opening = false;
	closing = true;
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
	case EPlayerColours::smiley:
		mesh->SetMaterial(0, smiley);
		break;
	case EPlayerColours::tux:
		mesh->SetMaterial(0, tux);
		break;
	case EPlayerColours::glasses:
		mesh->SetMaterial(0, glasses);
		break;
	case EPlayerColours::ironman:
		mesh->SetMaterial(0, ironMan);
		break;
	}
}
