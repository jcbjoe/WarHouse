// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawnPoint.h"
#include "WarhousePawn.h"
#include "PlayerManager.generated.h"

UCLASS()
class MYPROJECT_API APlayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
	TArray<APlayerSpawnPoint*> spawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
		int PlayerCount = 1;

	FVector GetRandomSpawnpoint()
	{
		int RandIndex = FMath::RandRange(0, spawnPoints.Num() - 1);
		return spawnPoints[RandIndex]->GetActorLocation();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<AWarhousePawn*> playerList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
