// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageSpawnActor.h"


// Sets default values
APackageSpawnActor::APackageSpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	const auto subObject = CreateDefaultSubobject<USceneComponent>(TEXT("PackageSpawnPoint"));
	RootComponent = subObject;

	////add billboard for editor visibility
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));
	BillboardComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APackageSpawnActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APackageSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
