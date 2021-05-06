// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseConveyorBeltTrigger.h"

// Sets default values
AWarhouseConveyorBeltTrigger::AWarhouseConveyorBeltTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//set up box collision
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->SetupAttachment(RootComponent);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseConveyorBeltTrigger::OnOverlapBegin);
	//boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseConveyorBeltTrigger::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(60, 50, 30));
	boxComponent->SetRelativeLocation(FVector(380, 0, 70));
}

// Called when the game starts or when spawned
void AWarhouseConveyorBeltTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWarhouseConveyorBeltTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarhouseConveyorBeltTrigger::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA(APackageBase::StaticClass()))
		{
			Package = Cast<APackageBase>(OtherActor);
			ResetPackagePosition();
		}
		else
			OtherActor->Destroy();
	}
}

void AWarhouseConveyorBeltTrigger::ResetPackagePosition()
{
	Package->SetActorLocation(SpawnActor->GetActorLocation());
}
