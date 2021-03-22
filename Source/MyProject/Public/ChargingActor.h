// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ChargingActor.generated.h"

UCLASS()
class MYPROJECT_API AChargingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChargingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* pad;


	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UParticleSystemComponent* beamEmitter;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UBoxComponent* boxComponent;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
