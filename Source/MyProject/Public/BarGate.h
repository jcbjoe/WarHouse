﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BarGate.generated.h"

UCLASS()
class MYPROJECT_API ABarGate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarGate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveUp();
	void MoveDown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BarHolder;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Bar;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* boxComponent;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	bool movingUp = false;
	bool movingDown = false;

	bool isUp = false;
	bool isDown = true;

	float rotationAmount = 0;

	const float movementSpeed = 100;
};
