// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "SpecialPackageSpawnActor.generated.h"

UCLASS()
class MYPROJECT_API ASpecialPackageSpawnActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpecialPackageSpawnActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//A UBillboardComponent to hold Icon sprite
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
		UBillboardComponent* BillboardComponent;
	//Sprite for the Billboard Component
	UTexture2D* SpriteTexture;
};
