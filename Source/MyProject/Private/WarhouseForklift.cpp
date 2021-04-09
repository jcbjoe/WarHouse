﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseForklift.h"

// Sets default values
AWarhouseForklift::AWarhouseForklift()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	FrontWheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontWheels"));
	FrontWheels->SetupAttachment(RootComponent);
	BackWheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWheels"));
	BackWheels->SetupAttachment(RootComponent);
	LiftBit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiftBit"));
	LiftBit->SetupAttachment(RootComponent);
	Blades = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blades"));
	Blades->SetupAttachment(RootComponent);
	Pallet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pallet"));
	Pallet->SetupAttachment(RootComponent);
}

void AWarhouseForklift::DeliverPackages()
{
	Stop();
}

void AWarhouseForklift::ResumeMovement()
{
	isMoving = true;
}

void AWarhouseForklift::GetReadyToDeliver()
{
	//spawn packages on pallet

	//set off
	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AWarhouseForklift::ResumeMovement, ForkliftWaitSeconds, false);
}

// Called when the game starts or when spawned
void AWarhouseForklift::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AWarhouseForklift::ResumeMovement, ForkliftWaitSeconds, false);
}

void AWarhouseForklift::MoveForklift(float deltaTime)
{
	isMoving = true;
	FVector Location = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	Location += Direction * Speed * deltaTime;
	SetActorLocation(Location);
}

void AWarhouseForklift::Stop()
{
	isMoving = false;
	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AWarhouseForklift::ResumeMovement, ForkliftWaitSeconds, false);
}

void AWarhouseForklift::RotateWheels()
{
	WheelRotation += 1;
	FrontWheels->SetRelativeRotation(FRotator::MakeFromEuler({ 0, -WheelRotation, 0 }));
	BackWheels->SetRelativeRotation(FRotator::MakeFromEuler({ 0, -WheelRotation, 0 }));
}

// Called every frame
void AWarhouseForklift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isMoving)
	{
		MoveForklift(DeltaTime);
		RotateWheels();
	}
}