// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PhysicsProp.generated.h"

UCLASS()
class MYPROJECT_API APhysicsProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhysicsProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PropMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool Destructible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool HasParticleEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool Fragile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool CanPickUp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
