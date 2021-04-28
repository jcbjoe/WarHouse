// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseConveyorBelt.h"
#include "Packagebase.h"
#include "PhysicsProp.h"
#include "DestructibleProp.h"

// Sets default values
AWarhouseConveyorBelt::AWarhouseConveyorBelt()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConveyorBelt(TEXT("/Game/Assets/ConorAssets/ConveyorBelt/Conveyer_Belt.Conveyer_Belt"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConveyorBelt"));
	RootComponent = BaseMesh;
	BaseMesh->SetStaticMesh(ConveyorBelt.Object);
	//box component
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseConveyorBelt::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseConveyorBelt::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(2025, 85, 115));
	boxComponent->SetRelativeLocation(FVector(0
		, 0, 70));
	//set speed
	BeltSpeed = 40.0f;
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
	MoveObjectOnBelt(DeltaTime);
}

void AWarhouseConveyorBelt::MoveObjectOnBelt(float DeltaTime)
{
	float Speed = BeltSpeed * DeltaTime;
	FVector Direction = (BaseMesh->GetForwardVector()) * Speed;
	//this needs looking at, causes an issue when removing package while checking the loop
	//not sure how to fix this?
	/*for (AActor* actor : OverlappingActors)
	{
		if (IsOverlappingActor(actor))
			actor->AddActorWorldOffset(Direction);
		else
			OverlappingActors.Remove(actor);
	}*/

	boxComponent->GetOverlappingActors(OverlappingActors);
	for (AActor* actor : OverlappingActors)
	{
		actor->AddActorWorldOffset(Direction);
	}
}

void AWarhouseConveyorBelt::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor->IsA(APackageBase::StaticClass()) || OtherActor->IsA(APhysicsProp::StaticClass()) || OtherActor->IsA(ADestructibleProp::StaticClass()))
	{
		OverlappingActors.Add(OtherActor);
	}*/
}

void AWarhouseConveyorBelt::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
