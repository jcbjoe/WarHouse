// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PackageBase.h"
#include "SpecialPackageSpawnActor.h"
#include "Components/BoxComponent.h"
#include "WarhouseConveyorBeltTrigger.generated.h"

UCLASS()
class MYPROJECT_API AWarhouseConveyorBeltTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarhouseConveyorBeltTrigger();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	UFUNCTION()
		void ResetPackagePosition();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Package")
		APackageBase* Package;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Actor")
		ASpecialPackageSpawnActor* SpawnActor;
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* boxComponent;
};
