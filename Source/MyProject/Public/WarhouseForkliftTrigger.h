﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WarhouseForklift.h"
#include "PackageBase.h"
#include "WarhouseForkliftTrigger.generated.h"

UCLASS()
class MYPROJECT_API AWarhouseForkliftTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseForkliftTrigger();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxComponent;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void StopForklift();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Purpose")
		bool IsTurningAround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Purpose")
		bool IsStopping;
	//reference to the forklift in the map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forklift")
		AWarhouseForklift* Forklift;
};
