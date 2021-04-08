// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "WarhouseForklift.generated.h"

UCLASS()
class MYPROJECT_API AWarhouseForklift : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseForklift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* FrontWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BackWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* LiftBit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Blades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Pallet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		float ForkliftWaitSeconds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		bool isMoving;

	float WheelRotation = 0;

	FTimerHandle ForkliftTimerHandle;

	void MoveForklift();
	void Stop();
	void RotateWheels(float deltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
