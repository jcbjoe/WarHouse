// Fill out your copyright notice in the Description page of Project Settings.


#include "BarGate.h"

// Sets default values
ABarGate::ABarGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> barGateMat(TEXT("/Game/Assets/JoeAssets/BarGate/BarGateMat.BarGateMat"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> barHolderMesh(TEXT("/Game/Assets/JoeAssets/BarGate/BarHolder.BarHolder"));
	BarHolder = CreateDefaultSubobject<UStaticMeshComponent>(FName("BarHolder"));
	BarHolder->SetStaticMesh(barHolderMesh.Object);
	BarHolder->SetMaterial(0, barGateMat.Object);

	RootComponent = BarHolder;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> barMesh(TEXT("/Game/Assets/JoeAssets/BarGate/Bar.Bar"));
	Bar = CreateDefaultSubobject<UStaticMeshComponent>(FName("Bar"));
	Bar->SetStaticMesh(barMesh.Object);
	Bar->SetMaterial(0, barGateMat.Object);
	Bar->SetRelativeLocation({ 0,34,153 });
	Bar->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABarGate::BeginPlay()
{
	Super::BeginPlay();

	MoveUp();

}

// Called every frame
void ABarGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (movingUp)
	{
		Bar->AddLocalRotation({ -0.75,0,0 });
		FRotator rot = Bar->GetRelativeRotation();
		if (rot.Pitch <= -90)
		{
			movingUp = false;
			isUp = true;
		}
	}

	if (movingDown)
	{
		Bar->AddLocalRotation({ 0.75,0,0 });
		FRotator rot = Bar->GetRelativeRotation();
		if (rot.Pitch >= 0)
		{
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
