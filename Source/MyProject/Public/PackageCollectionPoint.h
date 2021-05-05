// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PackageBase.h"
#include "PackageManager.h"
#include "DoorLight.h"
#include "PackageCollectionPoint.generated.h"

UCLASS()
class MYPROJECT_API APackageCollectionPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APackageCollectionPoint();
	
	void ButtonPressed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* boxComponent;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Multiplier")
		float PackageBonus = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Multiplier")
		int PackageCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Multiplier")
		ADoorLight* doorLight;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	bool platformMovingUp = false;

	bool platformAtTop = false;

	bool platformMovingDown = false;

	float moveIncrement = 0.5;

	float amountToMove = 163;

	FVector originalPos;

	float currentWaitTime = 0;
	float maxWaitTime = 5;

	bool packagesBeingRemoved = false;

	UAudioComponent* beepSound;
	UAudioComponent* liftSound;

	const float beepSoundMultipler = 0.75;
	const float liftSoundMultipler = 0.6;

	float volumeMultiplier = 1.0;

	TArray<APackageBase*> packages = TArray<APackageBase*>();
	TArray<AActor*> OverlappingActors;
};
