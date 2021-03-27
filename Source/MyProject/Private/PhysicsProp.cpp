﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProp.h"

// Sets default values
APhysicsProp::APhysicsProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	RootComponent = PropMesh;
}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APhysicsProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
