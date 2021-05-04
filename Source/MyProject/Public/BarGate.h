// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BarGate.generated.h"

UCLASS()
class MYPROJECT_API ABarGate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BarHolder;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Bar;

private:
	bool movingUp = false;
	bool movingDown = false;

	bool isUp = false;
	bool isDown = true;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveUp();
	void MoveDown();
};
