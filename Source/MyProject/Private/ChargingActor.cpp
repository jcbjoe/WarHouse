// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargingActor.h"
#include "WarhousePawn.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AChargingActor::AChargingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> chargingStand(TEXT("/Game/Assets/JoeAssets/ChargingStand/ChargingStand.ChargingStand"));
	// Create the mesh component
	pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChargingStand"));

	pad->SetStaticMesh(chargingStand.Object);

	pad->SetRelativeScale3D(FVector(2.269586, 2.269586, 2.2695860));

	RootComponent = pad;

	ConstructorHelpers::FObjectFinder<UParticleSystem> emitter(TEXT("/Game/Assets/JoeAssets/ChargingStand/ChargingStandEmitter.ChargingStandEmitter"));

	UParticleSystem* templateEmitter = emitter.Object;

	beamEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));
	beamEmitter->Template = templateEmitter;

	beamEmitter->SetRelativeLocation(FVector(0.0, 0.0, 11.171405));
	beamEmitter->SetRelativeScale3D(FVector(0.305169, 0.305169, 1.0));

	beamEmitter->SetupAttachment(RootComponent);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	boxComponent->SetRelativeLocation(FVector(0, 0, 47.0));

	boxComponent->InitBoxExtent(FVector(32.0, 32.0, 60.0));

	boxComponent->SetupAttachment(RootComponent);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AChargingActor::OnOverlapBegin);

	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AChargingActor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AChargingActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AChargingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChargingActor::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass()))
	{
		auto player = reinterpret_cast<AWarhousePawn*>(OtherActor);

		player->SetIsOnCharger(true);
	}
}

void AChargingActor::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass()))
	{
		auto player = reinterpret_cast<AWarhousePawn*>(OtherActor);

		player->SetIsOnCharger(false);
	}
}

