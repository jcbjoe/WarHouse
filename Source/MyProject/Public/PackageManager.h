// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PackageDataStruct.h"
#include "PackageManager.generated.h"

UCLASS()
class MYPROJECT_API APackageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APackageManager();
	//variables
	//need a tarray of spawn locations? or are we already having them in the level and activating them? meaning a tarray of packages

	//functions
	void GetPackageDetails();
	void SpawnPackage(FPackageDataStructure pds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FPackageDataStructure pds;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
