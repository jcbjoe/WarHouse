// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WarhouseConveyorBelt.generated.h"

UCLASS()
class MYPROJECT_API AWarhouseConveyorBelt : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseConveyorBelt();

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* boxComponent;
	UPROPERTY(EditDefaultsOnly)
		TArray<AActor*> OverlappingActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float BeltSpeed;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void MoveObjectOnBelt(float DeltaTime);
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
