// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"

#include "FloatingScore.generated.h"

UCLASS()
class MYPROJECT_API AFloatingScore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingScore();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetText(FText textToSet);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* base;

	UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* text;
};
