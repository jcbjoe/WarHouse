// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleComponent.h"
#include "GameFramework/DamageType.h"
#include "DestructibleProp.generated.h"

UCLASS()
class MYPROJECT_API ADestructibleProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleProp();

	void CanHit();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UDestructibleComponent* PropMesh;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void DestroyProp();
};
