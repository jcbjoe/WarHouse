// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerColours.h"
#include "GameFramework/Actor.h"
#include "Shutter.generated.h"

UCLASS()
class MYPROJECT_API AShutter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShutter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMesh* shutterOpen;
	UStaticMesh* shutterClosed;

	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isOpen = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Open();

	void Close();

	void SetColour(EPlayerColours colour);

	bool isShutterOpen() { return isOpen; }

private:
	UMaterial* red;
	UMaterial* blue;
	UMaterial* yellow;
	UMaterial* white;
};
