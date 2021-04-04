// Fill out your copyright notice in the Description page of Project Settings.
#include "PackageCollectionPoint.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PackageProgressBar.h"


#include "WarhouseHelpers.h"

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
	PackageManager = WarhouseHelpers::GetPackageManager(GetWorld());
}

// Called every frame
void APackageCollectionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APackageBase*> packagesToRemove;

	for (TPair<APackageBase*, float>& package : packages)
	{
		if (package.Key->GetHeldBy().Num() > 0)
		{
			package.Value = 0;
			package.Key->SetProgressBarVisability(false);
		}
		else
		{
			package.Value += DeltaTime;
			auto newVal = UKismetMathLibrary::MapRangeClamped(package.Value, 0, 5, 0, 1);
			package.Key->SetProgressBarFill(newVal);
			package.Key->SetProgressBarVisability(true);
			if (package.Value >= 5)
			{
				packagesToRemove.Add(package.Key);
				//remove package from Package Manager array of packages
				PackageManager->RemovePackage(package.Key);
				//if package array count is less than threshold, activate timer to spawn new packages
				if (PackageManager->GetPackagesLength() < PackageManager->PackageThreshold)
				{
					PackageManager->ActivatePackageTimer();
				}
			}
		}
	}

	for (auto packageToRemove : packagesToRemove)
	{
		const int packageValue = packageToRemove->GetPackageValue();
		packages.Remove(packageToRemove);
		packageToRemove->Destroy();

		AGameManager* manager = WarhouseHelpers::GetGameManager(GetWorld());

		int index = 0;
		bool found = true;
		for (APackageCollectionPoint* collectionPoint : manager->GetCollectionPoints())
		{
			if (collectionPoint == this)
			{
				found = true;
				break;
			}
			index++;
		}

		if (!found) throw std::exception("This packageCollectionPoint has not been instanced in the GameManager object!");

		manager->IncrementPlayerScore(index, packageValue);
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
		package->SetProgressBarVisability(false);
	}
}
