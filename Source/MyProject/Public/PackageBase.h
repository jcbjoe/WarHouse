// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PackageDataStruct.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraActor.h"
#include "PackageBase.generated.h"

UCLASS()
class MYPROJECT_API APackageBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageBase();

	void InitialisePackage(FConfigPackage pds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PackageMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
		UBoxComponent* collisionMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* meshMaterial;
	UPROPERTY(VisibleAnywhere, Category = "Package")
		FPackageDataStructure Package;

	int PackageValue;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
		UWidgetComponent* progressBar;

	bool isBeingHeld = false;

private:

	ACameraActor* cam;

};
