// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PackageCollectionPoint.h"
#include "CollectionPointButton.generated.h"

UCLASS()
class MYPROJECT_API ACollectionPointButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectionPointButton();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PressButton();

	void AButtonPressed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* base;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* boxComponent;

	UPROPERTY(EditDefaultsOnly)
		APackageCollectionPoint* packageCollectionPoint;

	UPROPERTY(EditDefaultsOnly)
		UBillboardComponent* BillboardComponent;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UAudioComponent* audioComp;

	float volumeMultiplier = 1.0;

	const float buttonPushSoundMultiplier = 0.5f;

	TArray<AWarhousePawn*> CollidingPlayers;
};
