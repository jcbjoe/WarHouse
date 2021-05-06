// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/SpotLight.h"
#include "Components/SpotLightComponent.h"
#include "DoorLight.generated.h"

UCLASS()
class MYPROJECT_API ADoorLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorLight();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpinning(bool val);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* lightMesh;

	UPROPERTY(EditDefaultsOnly)
		USpotLightComponent* light1;

	UPROPERTY(EditDefaultsOnly)
		USpotLightComponent* light2;
private:
	bool isSpinning = false;

	float light1Rotation = 0;
	float light2Rotation = 180;

	const float intensity = 160.0;
};
