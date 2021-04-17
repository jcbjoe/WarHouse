// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "WarhouseForklift.generated.h"

class UCurveFloat;

UCLASS()
class MYPROJECT_API AWarhouseForklift : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseForklift();

	UFUNCTION()
		void DeliverPackages();
	UFUNCTION()
		void ResumeMovement();
	UFUNCTION()
		void GetReadyToDeliver();
	UFUNCTION()
		void TimelineProgress(float value);

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
		UStaticMeshComponent* Blades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* LiftBit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Pallet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		float ForkliftWaitSeconds = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		bool isMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift Data")
		float Speed = 500.0f;

	float DefaultSpeed = 500.0f;

	float WheelRotation = 0;

	FTimerHandle ForkliftTimerHandle;
	//anim stuff
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* CurveFloat;
	UPROPERTY()
		FVector StartLocation;
	UPROPERTY()
		FVector EndLocation;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		float ZOffset;

	void Stop();
	void RotateWheels();
	void MoveForklift(float DeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
