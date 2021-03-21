// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "WarhousePawn.h"
#include "Kismet/GameplayStatics.h"

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

	for (int i = 0; i < PlayerCount; ++i)
	{

		APlayerController* playerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);

		const int rand = FMath::RandRange(0, tmpSpawns.Num() - 1);
		APlayerSpawnPoint* tmpSpawn = tmpSpawns[rand];
		AWarhousePawn* playerPawn = GetWorld()->SpawnActor<AWarhousePawn>(AWarhousePawn::StaticClass(), spawnParams);
		playerPawn->SetOwner(playerController);
		playerList.Add(playerPawn);

		playerPawn->SetActorLocation(tmpSpawn->GetActorLocation());
		tmpSpawns.Remove(tmpSpawn);

		playerController->Possess(playerPawn);
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

