// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialPackageSpawnActor.h"

// Sets default values
ASpecialPackageSpawnActor::ASpecialPackageSpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//set up root
	const auto subObject = CreateDefaultSubobject<USceneComponent>(TEXT("PackageSpawnPoint"));
	RootComponent = subObject;
	//add billboard for editor visibility
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));
	BillboardComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpecialPackageSpawnActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpecialPackageSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
