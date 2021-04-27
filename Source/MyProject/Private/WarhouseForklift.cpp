// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseForklift.h"
#include "WarhousePawn.h"
#include "SpecialPackageSpawnActor.h"

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

	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseForklift::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseForklift::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(120, 100, 60));
	boxComponent->SetRelativeLocation(FVector(380, 0, 70));

	PackageSpawn1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Spawn1"));
	PackageSpawn1->SetChildActorClass(ASpecialPackageSpawnActor::StaticClass());
	PackageSpawn2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Spawn2"));
	PackageSpawn2->SetChildActorClass(ASpecialPackageSpawnActor::StaticClass());
}

void AWarhouseForklift::DeliverPackages()
{
	Stop();
}

void AWarhouseForklift::ResumeMovement()
{
	isMoving = true;
	Speed = DefaultSpeed;
}

void AWarhouseForklift::GetReadyToDeliver()
{
	//set off
	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AWarhouseForklift::ResumeMovement, ForkliftWaitSeconds, false);
}

void AWarhouseForklift::TimelineProgress(float value)
{
	Location.Z = FMath::Lerp(GetActorLocation().Z, EndLocation.Z, value);
	//SetActorLocation(Location);
}

void AWarhouseForklift::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass()))
	{
		auto player = Cast<AWarhousePawn>(OtherActor);
		player->KillPlayer();
	}
}

void AWarhouseForklift::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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
		EndLocation.Z = ZOffset;

		CurveTimeline.PlayFromStart();
	}

	Speed = 0.0f;
	TurnAround = GetActorRotation();
}

void AWarhouseForklift::MoveForklift(float deltaTime)
{
	//isMoving = true;
	Location = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	Location += Direction * Speed * deltaTime;
	SetActorLocation(Location);
}

void AWarhouseForklift::RotateForklift()
{
	SetActorRotation(GetActorRotation() * -1);
}

void AWarhouseForklift::Stop()
{
	//	isMoving = false;
	Speed = 0.0f;
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
	CurveTimeline.TickTimeline(DeltaTime);
	//if (isMoving)
	//{
	MoveForklift(DeltaTime);
	RotateWheels();
	//}
}

void AWarhouseForklift::PrepareForkliftForAnotherDelivery()
{
	Stop();
	RotateForklift();
	//spawn packages
	//call timer to deliver packages
}
