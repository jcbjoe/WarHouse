// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PackageProgressBar.h"
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

	TArray<APackageBase*> packagesToRemove;

	for (TPair<APackageBase*, float>& package : packages)
	{
		if (package.Key->isBeingHeld)
		{
			package.Value = 0;
			package.Key->progressBar->SetVisibility(false);
		}
		else
		{
			package.Value += DeltaTime;
			auto newVal = UKismetMathLibrary::MapRangeClamped(package.Value, 0, 5, 0, 1);
			reinterpret_cast<UPackageProgressBar*>(package.Key->progressBar->GetUserWidgetObject())->progressBarFillAmount = newVal;
			package.Key->progressBar->SetVisibility(true);
			if (package.Value >= 5)
			{
				packagesToRemove.Add(package.Key);
			}
		}
	}

	for (auto packageToRemove : packagesToRemove)
	{
		packages.Remove(packageToRemove);
		packageToRemove->Destroy();

		TArray<AActor*> managers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), managers);

		AGameManager* manager = reinterpret_cast<AGameManager*>(managers[0]);

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
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = reinterpret_cast<APackageBase*>(OtherActor);
		if (packages.Contains(package)) return;
		packages.Add(package, 0);
	}
}

void APackageCollectionPoint::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = reinterpret_cast<APackageBase*>(OtherActor);
		packages.Remove(package);
		package->progressBar->SetVisibility(false);
	}
}