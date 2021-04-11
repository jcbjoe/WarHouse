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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* base;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxComponent;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		APackageCollectionPoint* packageCollectionPoint;

	void AButtonPressed();

	TArray<AWarhousePawn*> CollidingPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
		UBillboardComponent* BillboardComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PressButton();

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UAudioComponent* audioComp;
};
