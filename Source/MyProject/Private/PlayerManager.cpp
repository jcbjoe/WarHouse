// Fill out your copyright notice in the Description page of Project Settings.

#include "WarhousePawn.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h" 
#include "WarhouseHelpers.h"

#include "PlayerManager.h"
// Sets default values
APlayerManager::APlayerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const auto subObject = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerSpawnPoint"));

	RootComponent = subObject;
}

// Called when the game starts or when spawned
void APlayerManager::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerManager::SpawnPlayers()
{
	auto instance = WarhouseHelpers::GetGameInstance(GetWorld());

	bool instancePlayersFound = false;
	if (instance->playerInfo.Num() != 0) instancePlayersFound = true;

	TArray<AActor*> playerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), playerControllers);
	for (AActor* controller : playerControllers)
	{
		APlayerController* castedPlayer = reinterpret_cast<APlayerController*>(controller);
		UGameplayStatics::RemovePlayer(castedPlayer, true);
	}

	TArray<APlayerSpawnPoint*> tmpSpawns;
	for (auto spawnPoint : spawnPoints)
	{
		tmpSpawns.Add(spawnPoint);
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (instancePlayersFound) {
		for (auto playerInfo : instance->playerInfo)
		{
			APlayerController* playerController = UGameplayStatics::CreatePlayer(GetWorld(), playerInfo.controllerId, true);

			const int rand = FMath::RandRange(0, tmpSpawns.Num() - 1);
			APlayerSpawnPoint* tmpSpawn = tmpSpawns[rand];
			AWarhousePawn* playerPawn = GetWorld()->SpawnActor<AWarhousePawn>(AWarhousePawn::StaticClass(), spawnParams);
			playerPawn->SetOwner(playerController);
			playerList.Add(playerPawn);

			auto spawn = tmpSpawn->GetActorLocation();
			spawn.Z = 50;

			playerPawn->SetActorLocation(spawn);
			tmpSpawns.Remove(tmpSpawn);

			playerPawn->SetColour(playerInfo.colour);

			playerController->Possess(playerPawn);
		}
	}
	else
	{
		for (int i = 0; i < PlayerCount; ++i)
		{
			APlayerController* playerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);

			const int rand = FMath::RandRange(0, tmpSpawns.Num() - 1);
			APlayerSpawnPoint* tmpSpawn = tmpSpawns[rand];
			AWarhousePawn* playerPawn = GetWorld()->SpawnActor<AWarhousePawn>(AWarhousePawn::StaticClass(), spawnParams);
			playerPawn->SetOwner(playerController);
			playerList.Add(playerPawn);

			auto spawn = tmpSpawn->GetActorLocation();
			spawn.Z = 50;

			playerPawn->SetActorLocation(spawn);
			tmpSpawns.Remove(tmpSpawn);

			playerController->Possess(playerPawn);
		}
	}
}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AWarhousePawn*> APlayerManager::GetPlayers()
{
	return playerList;
}

FVector APlayerManager::GetRandomSpawnpoint(bool checkForPlayers)
{
	//--- If we don't want to check for players do a simple check
	if (!checkForPlayers)
	{
		int RandIndex = FMath::RandRange(0, spawnPoints.Num() - 1);
		return spawnPoints[RandIndex]->GetActorLocation();
	}

	//--- If we do want to check for players close by do the following
	bool spawnFound = false;
	FVector spawn;
	do
	{
		int RandIndex = FMath::RandRange(0, spawnPoints.Num() - 1);
		auto spawnPointLoc = spawnPoints[RandIndex]->GetActorLocation();

		bool playerToClose = false;
		for (AWarhousePawn* player : playerList)
		{
			if (FVector::Distance(player->GetActorLocation(), spawnPointLoc) < 20)
			{
				playerToClose = true;
				break;
			}
		}

		if (!playerToClose)
		{
			spawnFound = true;
			spawn = spawnPointLoc;
		}
	} while (!spawnFound);

	spawn.Z = 50;

	return spawn;
}