// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PackageDataStruct.h"
#include "Components/WidgetComponent.h"
#include "WarhousePawn.h"
//This is needed, Not sure why VS is saying its not
#include "Camera/CameraActor.h"
#include "PackageBase.generated.h"

UCLASS()
class MYPROJECT_API APackageBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PackageMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
		UBoxComponent* collisionMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* meshMaterial;
	UPROPERTY(EditAnywhere, Category = "Package")
		FConfigPackage Package;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
		UWidgetComponent* progressBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		float PackageHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		float PackageValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		bool IsBeingCollected = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Package")
		bool IsSpecial = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<AWarhousePawn*> heldBy = TArray<AWarhousePawn*>();

	USoundBase* soundBase;

	bool canRegisterHit = true;

	FTimerHandle timer;

public:

	void InitialisePackage(FConfigPackage pds);
	void StartHolding(AWarhousePawn* player);
	void EndHolding(AWarhousePawn* player);
	TArray<AWarhousePawn*> GetHeldBy() const;
	void SetProgressBarFill(float amount);
	void SetProgressBarVisability(bool visability);
	float GetPackageValue();
	float GetPackageWeight();
	float GetPackageHealth();
	bool GetIsBeingCollected();
	void SetIsBeingCollected(bool collected);
	void SetPackageSpecial();
	FConfigPackage GetPackageDetails();
	bool GetIsSpecial();
	void AllowHit();


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
