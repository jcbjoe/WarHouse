// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseForklift.h"

#include "SettingsSave.h"
#include "WarhousePawn.h"
#include "SpecialPackageSpawnActor.h"
#include "WarhouseHelpers.h"

// Sets default values
AWarhouseForklift::AWarhouseForklift()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> forkliftBase(TEXT("/Game/Assets/ConorAssets/Forklift/ForkliftBody_polySurface71.ForkliftBody_polySurface71"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetStaticMesh(forkliftBase.Object);
	RootComponent = BaseMesh;
	//Wheels
	static ConstructorHelpers::FObjectFinder<UStaticMesh> forkliftWheels(TEXT("/Game/Assets/ConorAssets/Forklift/ForkliftWheels.ForkliftWheels"));

	FrontWheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontWheels"));
	FrontWheels->SetStaticMesh(forkliftWheels.Object);
	FrontWheels->SetRelativeLocation({ 103,0,25 });
	FrontWheels->SetupAttachment(RootComponent);
	FrontWheels->SetGenerateOverlapEvents(false);

	BackWheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWheels"));
	BackWheels->SetStaticMesh(forkliftWheels.Object);
	BackWheels->SetRelativeLocation({ -111,0,25 });
	BackWheels->SetupAttachment(RootComponent);
	BackWheels->SetGenerateOverlapEvents(false);
	//lift bit
	static ConstructorHelpers::FObjectFinder<UStaticMesh> forkliftLift(TEXT("/Game/Assets/ConorAssets/Forklift/ForkliftBody_polySurface82.ForkliftBody_polySurface82"));
	LiftBit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiftBit"));
	LiftBit->SetStaticMesh(forkliftLift.Object);
	LiftBit->SetupAttachment(RootComponent);
	LiftBit->SetGenerateOverlapEvents(false);
	//blades
	static ConstructorHelpers::FObjectFinder<UStaticMesh> forkliftForks(TEXT("/Game/Assets/ConorAssets/Forklift/ForkliftBody_polySurface81.ForkliftBody_polySurface81"));
	Blades = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blades"));
	Blades->SetStaticMesh(forkliftForks.Object);
	Blades->SetupAttachment(RootComponent);
	Blades->SetGenerateOverlapEvents(false);
	//pallet
	static ConstructorHelpers::FObjectFinder<UStaticMesh> palletObj(TEXT("/Game/Assets/ConorAssets/Pallet/pallet.pallet"));
	Pallet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pallet"));
	Pallet->SetStaticMesh(palletObj.Object);
	Pallet->SetRelativeLocation({ 360,0.3,9 });
	Pallet->SetRelativeRotation(FRotator::MakeFromEuler({ 0,0,-90 }));
	Pallet->SetupAttachment(RootComponent);
	Pallet->SetGenerateOverlapEvents(false);
	//box component
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->SetupAttachment(RootComponent);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseForklift::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseForklift::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(120, 100, 60));
	boxComponent->SetRelativeLocation(FVector(380, 0, 70));
	//components to spawn packages at their location
	PackageSpawn1 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn1"));
	PackageSpawn1->SetRelativeLocation({ 335,-45,200 });
	PackageSpawn1->SetupAttachment(RootComponent);

	PackageSpawn2 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn2"));
	PackageSpawn2->SetRelativeLocation({ 435,45,200 });
	PackageSpawn2->SetupAttachment(RootComponent);
	//set up audio
	static ConstructorHelpers::FObjectFinder<USoundWave> engineSound(TEXT("/Game/Sounds/forklift-engine.forklift-engine"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("Audio"));
	AudioComponent->SetSound(engineSound.Object);
	AudioComponent->SetVolumeMultiplier(0.0f);
	AudioComponent->SetupAttachment(RootComponent);
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
}

void AWarhouseForklift::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass()) && isMoving)
	{
		auto player = Cast<AWarhousePawn>(OtherActor);
		player->KillPlayer();
	}

	if (OtherActor->IsA(APackageBase::StaticClass()))
	{
		AddPackageToArray((APackageBase*)OtherActor);
	}
}

void AWarhouseForklift::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APackageBase::StaticClass()))
	{
		PackagesToRemove.RemoveSingle((APackageBase*)OtherActor);
	}
}

// Called when the game starts or when spawned
void AWarhouseForklift::BeginPlay()
{
	Super::BeginPlay();
	//for bobbing the forkilft up and down
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
	//prevent forklift from moving before its needed
	Speed = 0.0f;
	//activate overlap events on collision box
	boxComponent->SetGenerateOverlapEvents(true);
	//apply user settings
	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		volumeMultiplier = LoadedGame->SFXVolume;
	}
	//get refernce to PAckageManager
	auto packageManager = WarhouseHelpers::GetPackageManager(GetWorld());
	//play the audio
	AudioComponent->Play();
	//spawn two special packages on the pallet
	packageManager->SpawnSpecialPackage(packageManager->GetConfig(), PackageSpawn1->GetComponentLocation());
	packageManager->SpawnSpecialPackage(packageManager->GetConfig(), PackageSpawn2->GetComponentLocation());
}

void AWarhouseForklift::MoveForklift(float deltaTime)
{
	isMoving = true;
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
	isMoving = false;
	Speed = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AWarhouseForklift::ResumeMovement, ForkliftWaitSeconds, false);
	AudioComponent->Stop();
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
	if (isMoving) {
		MoveForklift(DeltaTime);
		RotateWheels();
		AudioComponent->SetVolumeMultiplier(engineSoundVolume * volumeMultiplier);
	}
	else
	{
		AudioComponent->SetVolumeMultiplier(0.0f);
	}
}

void AWarhouseForklift::PrepareForkliftForAnotherDelivery()
{
	Stop();
	//remove any special packages still on the forklift by looping through a tarray
	RemoveAndDestroyPackages();
	RotateForklift();
	//spawn special packages on forklift + conveyor
	auto packageManager = WarhouseHelpers::GetPackageManager(GetWorld());
	packageManager->SpawnSpecialPackage(packageManager->GetConfig(), PackageSpawn1->GetComponentLocation());
	packageManager->SpawnSpecialPackage(packageManager->GetConfig(), PackageSpawn2->GetComponentLocation());

}

void AWarhouseForklift::AddPackageToArray(APackageBase* package)
{
	PackagesToRemove.Add(package);
}

void AWarhouseForklift::RemoveAndDestroyPackages()
{
	//loop through PackagesToRemove
	auto packageManager = WarhouseHelpers::GetPackageManager(GetWorld());
	for (int i = 0; i < PackagesToRemove.Num(); i++)
	{
		APackageBase* package = PackagesToRemove[i];
		//remove this package from array in package manager
		packageManager->RemovePackage(package);
		//remove from packages to remove
		PackagesToRemove.RemoveSingle(package);
		//destroy this package
		package->Destroy();
	}
}
