// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PropMeshComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool IsDestructible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool UseParticleEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool IsFragile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool CanPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Options")
		bool CanExplode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float PropHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float ImpactRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float RadialImpactForce;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		USoundBase* soundBase;

	//for deactivating particles after a set time
	FTimerHandle ParticlesTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data")
		float ParticleLife = 3.0f;

	bool isParticleSystemActive = false;
	bool isPropDead = false;
	//for checking who is hit by the radial force
	TArray<FHitResult> HitActors;
	FTimerHandle Timer;
	FTimerHandle DestroySelfTimer;
	float SelfDestroyTime = 1.0f;
	bool canRegisterHit = true;

	void AllowHit();

private:
	float volumeMultiplier = 0.0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCanPickUp();
	bool GetDestructible();
	bool GetUseParticleEmitter();
	bool GetIsFragile();
	bool GetCanExplode();
	UFUNCTION()
		void ActivateParticles();
	UFUNCTION()
		void DeactivateParticles();
	UFUNCTION()
		void DestroyProp();
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void Explode();
	UFUNCTION()
		void DestroySelf();
};
