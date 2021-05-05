// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseForkliftTrigger.h"

// Sets default values
AWarhouseForkliftTrigger::AWarhouseForkliftTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//box component
	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxComponent->SetWorldLocation(GetActorLocation());
	boxComponent->SetupAttachment(RootComponent);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarhouseForkliftTrigger::OnOverlapBegin);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWarhouseForkliftTrigger::OnOverlapEnd);
	boxComponent->SetBoxExtent(FVector(158, 128, 60));
	boxComponent->SetRelativeLocation(FVector(380, 0, 70));
}

// Called when the game starts or when spawned
void AWarhouseForkliftTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWarhouseForkliftTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarhouseForkliftTrigger::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		//get the name of the component that is overlapping
		FString otherComponentName = OtherComp->GetName();
		if (OtherActor->IsA(AWarhouseForklift::StaticClass()) && IsStopping)
		{
			//only stop forklift if it is the base mesh (prevents multiple colliders activating this functionlaity)
			if (otherComponentName == "BaseMesh") {
				StopForklift();
			}
		}

		if (OtherActor->IsA(AWarhouseForklift::StaticClass()) && IsTurningAround)
		{
			//only stop forklift if it is the base mesh (prevents multiple colliders activating this functionlaity)
			if (otherComponentName == "BaseMesh") {
				Forklift->PrepareForkliftForAnotherDelivery();
			}
		}
	}
}

void AWarhouseForkliftTrigger::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWarhouseForkliftTrigger::StopForklift()
{
	Forklift->Stop();
}
