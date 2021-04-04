// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PackageProgressBar.h"
#include "WarhouseHelpers.h"
#include "PackageCollectionPoint.h"

// Sets default values
APackageCollectionPoint::APackageCollectionPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<UStaticMeshComponent>(FName("Platform"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> platformMesh(TEXT("/Game/Assets/ConorAssets/PackagePlatform.PackagePlatform"));

	base->SetStaticMesh(platformMesh.Object);

	RootComponent = base;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	boxComponent->SetWorldLocation(GetActorLocation());

	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &APackageCollectionPoint::OnOverlapBegin);

	boxComponent->OnComponentEndOverlap.AddDynamic(this, &APackageCollectionPoint::OnOverlapEnd);

	boxComponent->SetBoxExtent(FVector(240, 160, 60));
	boxComponent->SetRelativeLocation(FVector(0, 0, 70));

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


	if (platformMovingUp)
	{
		auto actorPos = GetActorLocation();

		actorPos.Z += moveIncrement;

		SetActorLocation(actorPos);

		if ((actorPos.Z - originalPos.Z) >= amountToMove)
		{
			platformMovingUp = false;
			platformAtTop = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("At Top"));
		}
	}
	else if (platformAtTop)
	{

		currentWaitTime += DeltaTime;

		if (currentWaitTime > maxWaitTime)
		{
			packagesBeingRemoved = true;
			for (auto packageToRemove : packages)
			{
				const int packageValue = packageToRemove->GetPackageValue();
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

			packages.Empty();

			packagesBeingRemoved = false;

			currentWaitTime = 0;
			platformAtTop = false;
			platformMovingDown = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Platform Going down"));
		}

	}
	else if (platformMovingDown)
	{
		auto actorPos = GetActorLocation();

		actorPos.Z -= moveIncrement;

		SetActorLocation(actorPos);

		if ((actorPos.Z - originalPos.Z) <= 0)
		{
			platformMovingDown = false;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Platform at bottom"));
		}
	}

}

void APackageCollectionPoint::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = reinterpret_cast<APackageBase*>(OtherActor);
		if (packages.Contains(package)) return;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Adding package"));
		packages.Add(package);
	}
}

void APackageCollectionPoint::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = reinterpret_cast<APackageBase*>(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Removing package"));
		if (!packagesBeingRemoved)
			packages.Remove(package);
	}
}

void APackageCollectionPoint::ButtonPressed()
{

	if (platformMovingUp || platformAtTop || platformMovingDown) return;

	originalPos = GetActorLocation();

	platformMovingUp = true;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Moving Up"));
}
