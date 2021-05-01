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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
		TArray<APlayerSpawnPoint*> spawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
		int PlayerCount = 1;

	FVector GetRandomSpawnpoint(bool checkForPlayers = false);

	TArray<AWarhousePawn*> GetPlayers();

	void SpawnPlayers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const float PlayerHeight = 25;
private:
	TArray<AWarhousePawn*> playerList;
};
