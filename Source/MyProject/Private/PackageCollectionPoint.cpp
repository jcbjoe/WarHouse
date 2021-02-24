// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "PackageCollectionPoint.h"

// Sets default values
APackageCollectionPoint::APackageCollectionPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	RootComponent = base;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	boxComponent->SetWorldLocation(GetActorLocation());

	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &APackageCollectionPoint::OnOverlapBegin);

	boxComponent->OnComponentEndOverlap.AddDynamic(this, &APackageCollectionPoint::OnOverlapEnd);

}

// Called when the game starts or when spawned
void APackageCollectionPoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APackageCollectionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AWarehousePackage*> packagesToRemove;

	for (auto package : packages)
	{
		if (package.Key->isBeingHeld)
		{
			package.Value = FDateTime::Now();
		}
		else
		{
			auto difference = (FDateTime::Now() - package.Value).GetSeconds();
			if (difference > 5)
			{
				packagesToRemove.Add(package.Key);
			}
		}
	}

	for (auto packageToRemove : packagesToRemove)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Package Destroyed"));
		packages.Remove(packageToRemove);
		packageToRemove->Destroy();

		TArray<AActor*> managers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), managers);

		AGameManager* manager = (AGameManager*)managers[0];

		int index = 0;
		bool found = true;
		for (auto collectionPoint : manager->packageCollectionPoints)
		{
			if (collectionPoint == this)
			{
				found = true;
				break;
			}
			index++;
		}

		if (!found) throw std::exception("This packageCollectionPoint has not been instanced in the GameManager object!");
		else manager->IncrementPlayerScore(index);
	}

	packagesToRemove.Empty();

}

void APackageCollectionPoint::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarehousePackage::StaticClass())) {
		auto date = FDateTime::Now();
		auto package = reinterpret_cast<AWarehousePackage*>(OtherActor);
		if (packages.Contains(package)) return;
		packages.Add(package, date);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Package added"));
	}
}

void APackageCollectionPoint::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AWarehousePackage::StaticClass())) {
		auto package = reinterpret_cast<AWarehousePackage*>(OtherActor);
		packages.Remove(package);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Package removed"));
	}
}