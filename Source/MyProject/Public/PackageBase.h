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

	void InitialisePackage(FConfigPackage pds);
	
	void StartHolding(AWarhousePawn* player);
	
	void EndHolding(AWarhousePawn* player);
	
	TArray<AWarhousePawn*> GetHeldBy() const;
	
	float GetPackageValue();
	
	float GetPackageWeight();
	
	float GetPackageHealth();
	
	bool GetIsBeingCollected();
	
	void SetIsBeingCollected(bool collected);
	
	void SetPackageSpecial();
	
	FConfigPackage GetPackageDetails();
	
	bool GetIsSpecial();
	
	void AllowHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//--- Components
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* PackageMesh;
	
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* collisionMesh = nullptr;

	//-- Editor settings
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterial* meshMaterial;
	UPROPERTY(EditAnywhere, Category = "Package")
		FConfigPackage Package;

	UPROPERTY(EditAnywhere, Category = "Package")
		float PackageHealth;
	UPROPERTY(EditAnywhere, Category = "Package")
		float PackageValue;
	UPROPERTY(EditAnywhere, Category = "Package")
		bool IsBeingCollected = false;
	UPROPERTY(EditAnywhere, Category = "Package")
		bool IsSpecial = false;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
private:
	float volumeMultiplier = 1.0;

	const float packageDropSoundMultiplier = 0.5;

	TArray<AWarhousePawn*> heldBy = TArray<AWarhousePawn*>();

	USoundBase* soundBase;

	bool canRegisterHit = true;

	FTimerHandle timer;
};
