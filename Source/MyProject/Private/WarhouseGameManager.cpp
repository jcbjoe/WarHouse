// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseGameManager.h"

// Sets default values
AWarhouseGameManager::AWarhouseGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	////add billboard for editor visibility
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));
	BillboardComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AWarhouseGameManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWarhouseGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarhouseGameManager::SpawnPackage()
{

}
