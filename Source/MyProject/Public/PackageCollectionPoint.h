// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PackageBase.h"
#include "PackageManager.h"
#include "PackageCollectionPoint.generated.h"

UCLASS()
class MYPROJECT_API APackageCollectionPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageCollectionPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APackageManager* PackageManager;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxComponent;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* base;


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<APackageBase*> packages = TArray<APackageBase*>();


	void ButtonPressed();

private:
	bool platformMovingUp = false;

	bool platformAtTop = false;

	bool platformMovingDown = false;

	float moveIncrement = 0.1;

	float amountToMove = 200;

	FVector originalPos;

	float currentWaitTime = 0;
	float maxWaitTime = 5;

	bool packagesBeingRemoved = false;
};
