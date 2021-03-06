﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Internationalization/Text.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WarhouseClock.generated.h"

UCLASS()
class MYPROJECT_API AWarhouseClock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseClock();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTime(int time);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//properties
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* ClockMesh;
	UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* TimeText;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* base;
};
