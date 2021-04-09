// Fill out your copyright notice in the Description page of Project Settings.


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
}

void AWarhouseForklift::TimelineProgress(float value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, value);
	//SetActorLocation(NewLocation);
	BaseMesh->SetWorldLocation(NewLocation);
}

// Called when the game starts or when spawned
void AWarhouseForklift::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(true);

		StartLocation = EndLocation = GetActorLocation();
		EndLocation.Z = OffsetZ;

		CurveTimeline.PlayFromStart();
	}
}

void AWarhouseForklift::MoveForklift(float deltaTime)
{
	FVector Location = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	Location += Direction * Speed * deltaTime;
	SetActorLocation(Location);
}

void AWarhouseForklift::Stop()
{
	isMoving = false;
}

void AWarhouseForklift::RotateWheels()
{
	WheelRotation += 1;
	FrontWheels->SetRelativeRotation(FRotator::MakeFromEuler({ 0, -WheelRotation, 0 }));
	BackWheels->SetRelativeRotation(FRotator::MakeFromEuler({ 0, -WheelRotation, 0 }));
}

void AWarhouseForklift::WobbleBody()
{
	FVector Location = GetActorLocation();
}

// Called every frame
void AWarhouseForklift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
	if (isMoving)
	{
		MoveForklift(DeltaTime);
		RotateWheels();
	}
}
