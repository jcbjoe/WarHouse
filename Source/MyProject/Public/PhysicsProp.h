// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
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
		bool IsDestructible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool UseParticleEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool IsFragile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool CanPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float PropHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Extras")
		UParticleSystemComponent* ParticleSystem;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCanPickUp();
	bool GetDestructible();
	bool GetUseParticleEmitter();
	bool GetIsFragile();

};
