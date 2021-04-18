// Fill out your copyright notice in the Description page of Project Settings.

#include "TruckPackageManager.h"
#include "WarhouseHelpers.h"

// Sets default values
ATruckPackageManager::ATruckPackageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATruckPackageManager::BeginPlay()
{
	Super::BeginPlay();

	SetPackagesHidden();
}

// Called every frame
void ATruckPackageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATruckPackageManager::IncrementTruckStage(int truckNumber)
{
	switch (truckNumber)
	{
	case 1:
		truck1Stage++;
		break;
	case 2:
		truck2Stage++;
		break;
	case 3:
		truck3Stage++;
		break;
	case 4:
		truck4Stage++;
		break;
	}

	UpdateTrucks();
}

void ATruckPackageManager::UpdateTrucks()
{
	SetPackagesHidden();

	for (int i = 0; i < truck1Stage; i++)
	{
		Truck1PackageStages[i]->GetRootComponent()->SetVisibility(true);
	}

	for (int i = 0; i < truck2Stage; i++)
	{
		Truck2PackageStages[i]->GetRootComponent()->SetVisibility(true);
	}

	for (int i = 0; i < truck3Stage; i++)
	{
		Truck3PackageStages[i]->GetRootComponent()->SetVisibility(true);
	}

	for (int i = 0; i < truck4Stage; i++)
	{
		Truck4PackageStages[i]->GetRootComponent()->SetVisibility(true);
	}
}

void ATruckPackageManager::SetPackagesHidden()
{
	for (auto packageStage : Truck1PackageStages)
	{
		packageStage->GetRootComponent()->SetVisibility(false);
	}

	for (auto packageStage : Truck2PackageStages)
	{
		packageStage->GetRootComponent()->SetVisibility(false);
	}

	for (auto packageStage : Truck3PackageStages)
	{
		packageStage->GetRootComponent()->SetVisibility(false);
	}

	for (auto packageStage : Truck4PackageStages)
	{
		packageStage->GetRootComponent()->SetVisibility(false);
	}
}

void ATruckPackageManager::CheckIfFull()
{
	if (truck1Stage == 4)
	{
		EmptyTruck(1);
	}

	if (truck2Stage == 4)
	{
		EmptyTruck(2);
	}

	if (truck3Stage == 4)
	{
		EmptyTruck(3);
	}

	if (truck4Stage == 4)
	{
		EmptyTruck(4);
	}
}

void ATruckPackageManager::EmptyTruck(int truckNumber)
{
	WarhouseHelpers::GetGameManager(GetWorld())->shutters[truckNumber - 1]->Close();

	switch (truckNumber)
	{
	case 1:
		truck1Stage = 0;
		break;
	case 2:
		truck2Stage = 0;
		break;
	case 3:
		truck3Stage = 0;
		break;
	case 4:
		truck4Stage = 0;
		break;
	}

	UpdateTrucks();

	switch (truckNumber)
	{
	case 1:
		GetWorldTimerManager().SetTimer(truck1Handle, this, &ATruckPackageManager::OpenShutterTruck1, secondsTillShutterOpens, false);
		break;
	case 2:
		GetWorldTimerManager().SetTimer(truck2Handle, this, &ATruckPackageManager::OpenShutterTruck2, secondsTillShutterOpens, false);
		break;
	case 3:
		GetWorldTimerManager().SetTimer(truck3Handle, this, &ATruckPackageManager::OpenShutterTruck3, secondsTillShutterOpens, false);
		break;
	case 4:
		GetWorldTimerManager().SetTimer(truck4Handle, this, &ATruckPackageManager::OpenShutterTruck4, secondsTillShutterOpens, false);
		break;
	}

}

void ATruckPackageManager::OpenShutterTruck1()
{
	WarhouseHelpers::GetGameManager(GetWorld())->shutters[0]->Open();
}

void ATruckPackageManager::OpenShutterTruck2()
{
	WarhouseHelpers::GetGameManager(GetWorld())->shutters[1]->Open();
}

void ATruckPackageManager::OpenShutterTruck3()
{
	WarhouseHelpers::GetGameManager(GetWorld())->shutters[2]->Open();
}

void ATruckPackageManager::OpenShutterTruck4()
{
	WarhouseHelpers::GetGameManager(GetWorld())->shutters[3]->Open();
}
