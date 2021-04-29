﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool UseParticleEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool CanExplode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Extras")
		UParticleSystemComponent* ParticleSystem;

	//for deactivating particles after a set time
	FTimerHandle ParticlesTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float ParticleLife = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float ImpactRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float RadialImpactForce;
	//for checking who is hit by the radial force
	TArray<FHitResult> HitActors;

	bool isParticleSystemActive = false;
	bool isPropDead = false;
	bool canRegisterHit;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool GetUseParticleEmitter();
	bool GetCanExplode();
	UFUNCTION()
		void ActivateParticles();
	UFUNCTION()
		void DeactivateParticles();
	UFUNCTION()
		void DestroyProp();
	UFUNCTION()
		void Explode();
	/*UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/
};
