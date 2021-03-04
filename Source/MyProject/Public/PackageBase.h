// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PackageDataStruct.h"
#include "Math/UnrealMathUtility.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		FString PackageName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		FString ModelReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		TArray<int> ValueRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		int32 Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		float PackageWeight;

	int PackageValue;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
