// Fill out your copyright notice in the Description page of Project Settings.
#include "PackageCollectionPoint.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PackageProgressBar.h"
#include "WarhouseHelpers.h"
#include "Components/AudioComponent.h"

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

	static ConstructorHelpers::FObjectFinder<USoundWave> beepSoundObj(TEXT("/Game/Sounds/beep.beep"));

	beepSound = CreateDefaultSubobject<UAudioComponent>(FName("beepAudio"));

	beepSound->SetVolumeMultiplier(0.0f);

	beepSound->SetSound(beepSoundObj.Object);

	static ConstructorHelpers::FObjectFinder<USoundWave> liftSoundObj(TEXT("/Game/Sounds/lift.lift"));

	liftSound = CreateDefaultSubobject<UAudioComponent>(FName("liftAudio"));

	liftSound->SetVolumeMultiplier(0.0f);

	liftSound->SetSound(liftSoundObj.Object);
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
		doorLight->SetSpinning(true);

		auto actorPos = GetActorLocation();

		actorPos.Z += moveIncrement;

		SetActorLocation(actorPos);

		if ((actorPos.Z - originalPos.Z) >= amountToMove)
		{
			platformMovingUp = false;
			platformAtTop = true;

			liftSound->Stop();
		}
	}
	else if (platformAtTop)
	{

		currentWaitTime += DeltaTime;

		if (currentWaitTime > maxWaitTime)
		{
			auto packageManager = WarhouseHelpers::GetPackageManager(GetWorld());
			packagesBeingRemoved = true;
			int index = 0;
			bool found = true;

			AGameManager* manager = WarhouseHelpers::GetGameManager(GetWorld());
			
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

			//--- Wait for the door to reopen
			if (!manager->shutters[index]->isShutterOpen()) return;
			
			for (auto packageToRemove : packages)
			{
				//divide package health by 100 to get the percentage of what it is worth i.e. more damge, less value
				const float packageValue = (packageToRemove->GetPackageValue()) * PackageBonus;
				packageManager->RemovePackage(packageToRemove);
				packageToRemove->Destroy();

				float packageDamageCost = packageToRemove->GetPackageValue() * ((100 - packageToRemove->GetPackageHealth()) / 100.0f);
				
				manager->AddToPlayerScore(index, packageValue, packageDamageCost);
			}

			packages.Empty();

			WarhouseHelpers::GetTruckPackageManager(GetWorld())->IncrementTruckStage(index+1);

			packagesBeingRemoved = false;

			currentWaitTime = 0;
			platformAtTop = false;
			platformMovingDown = true;

			liftSound->Play();

			packageManager->ActivatePackageTimer();
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
			doorLight->SetSpinning(false);

			beepSound->Stop();
			liftSound->Stop();
			WarhouseHelpers::GetTruckPackageManager(GetWorld())->CheckIfFull();
		}
	}

}

void APackageCollectionPoint::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = Cast<APackageBase>(OtherActor);
		if (packages.Contains(package)) return;
		packages.Add(package);
		package->SetIsBeingCollected(true);
		PackageCounter += 1;
		if (PackageCounter > 1)
		{
			switch (package->GetPackageDetails().PackageSizeID)
			{
			case 1://big
				PackageBonus += 0.4f;
				break;
			case 2://med
				PackageBonus += 0.2f;
				break;
			case 3://small
				PackageBonus += 0.1f;
				break;
			case 4://that other one - long?
				PackageBonus += 0.3f;
				break;
			}
		}


	}
}

void APackageCollectionPoint::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(APackageBase::StaticClass())) {
		auto package = Cast<APackageBase>(OtherActor);
		if (!packagesBeingRemoved)
		{
			packages.Remove(package);
			package->SetIsBeingCollected(false);
			PackageCounter -= 1;
			if (PackageCounter > 1)
			{
				switch (package->GetPackageDetails().PackageSizeID)
				{
				case 1://big
					PackageBonus -= 0.4f;
					break;
				case 2://med
					PackageBonus -= 0.2f;
					break;
				case 3://small
					PackageBonus -= 0.1f;
					break;
				case 4://that other one - long?
					PackageBonus -= 0.3f;
					break;
				}
			}

		}
	}
}

void APackageCollectionPoint::ButtonPressed()
{

	if (platformMovingUp || platformAtTop || platformMovingDown) return;

	originalPos = GetActorLocation();

	platformMovingUp = true;

	beepSound->SetVolumeMultiplier(0.75);
	beepSound->Play();
	liftSound->SetVolumeMultiplier(0.6);
	liftSound->Play();
}
