// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraActor.h"
#include "WarehousePackage.generated.h"


UCLASS()
class MYPROJECT_API AWarehousePackage : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWarehousePackage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube")
	UStaticMeshComponent* cubeMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
		UBoxComponent* collisionMesh = nullptr;

	bool isBeingHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
		UWidgetComponent* progressBar;

private:

	ACameraActor* cam;


};
