// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseConveyorBelt.h"

// Sets default values
AWarhouseConveyorBelt::AWarhouseConveyorBelt()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	//box component
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseConveyorBelt::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseConveyorBelt::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(158, 128, 60));
	boxComponent->SetRelativeLocation(FVector(380, 0, 70));
}

// Called when the game starts or when spawned
void AWarhouseConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWarhouseConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarhouseConveyorBelt::MoveObjectOnBelt()
{

}

void AWarhouseConveyorBelt::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappingActors.Add(OtherActor);
}

void AWarhouseConveyorBelt::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActors.Remove(OtherActor);
}
