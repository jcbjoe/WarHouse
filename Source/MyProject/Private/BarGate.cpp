﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BarGate.h"

#include "PackageCollectionPoint.h"
#include "WarhouseForklift.h"

// Sets default values
ABarGate::ABarGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//--- Object imports
	static ConstructorHelpers::FObjectFinder<UMaterial> barGateMat(TEXT("/Game/Assets/JoeAssets/BarGate/BarGateMat.BarGateMat"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> barHolderMesh(TEXT("/Game/Assets/JoeAssets/BarGate/BarHolder.BarHolder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> barMesh(TEXT("/Game/Assets/JoeAssets/BarGate/Bar.Bar"));

	//--- Barholder setup
	BarHolder = CreateDefaultSubobject<UStaticMeshComponent>(FName("BarHolder"));
	BarHolder->SetStaticMesh(barHolderMesh.Object);
	BarHolder->SetMaterial(0, barGateMat.Object);

	RootComponent = BarHolder;

	//--- Bar setup
	Bar = CreateDefaultSubobject<UStaticMeshComponent>(FName("Bar"));
	Bar->SetStaticMesh(barMesh.Object);
	Bar->SetMaterial(0, barGateMat.Object);
	Bar->SetRelativeLocation({ 0,34,153 });
	Bar->SetupAttachment(RootComponent);

	//--- Box component (collision) setup
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->SetupAttachment(RootComponent);
	boxComponent->SetBoxExtent({ 230,250,100 });
	boxComponent->SetRelativeLocation({ -260,0,100 });

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABarGate::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &ABarGate::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ABarGate::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABarGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (movingUp)
	{
		rotationAmount -= movementSpeed * DeltaTime;
		Bar->SetRelativeRotation(FRotator::MakeFromEuler({ 0,rotationAmount,0 }));
		if (rotationAmount <= -90)
		{
			rotationAmount = -90;
			Bar->SetRelativeRotation(FRotator::MakeFromEuler({ 0,rotationAmount,0 }));
			movingUp = false;
			isUp = true;
		}
	}

	if (movingDown)
	{
		rotationAmount += movementSpeed * DeltaTime;
		Bar->SetRelativeRotation(FRotator::MakeFromEuler({ 0,rotationAmount,0 }));
		if (rotationAmount >= 0)
		{
			rotationAmount = 0;
			Bar->SetRelativeRotation(FRotator::MakeFromEuler({ 0,rotationAmount,0 }));
			movingDown = false;
			isDown = true;
		}
	}

}

void ABarGate::MoveUp()
{
	if (isUp || movingUp) return;

	isDown = false;
	movingUp = true;
	movingDown = false;
}

void ABarGate::MoveDown()
{
	if (isDown || movingDown) return;

	isUp = false;
	movingDown = true;
	movingUp = false;
}

void ABarGate::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhouseForklift::StaticClass())) {
		MoveUp();
	}
}

void ABarGate::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AWarhouseForklift::StaticClass())) {
		MoveDown();
	}
}
