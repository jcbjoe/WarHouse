// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "PackageSpawnActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	RootComponent = base;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	//add player scores to array
	playerScores.Add(player0Score);
	playerScores.Add(player1Score);
	playerScores.Add(player2Score);
	playerScores.Add(player3Score);

	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AGameManager::OnGameEnd, GameTimer, false);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameManager::IncrementPlayerScore(int playerIndex, int amount)
{
	switch (playerIndex)
	{
	case 0:
		player0Score += amount;
		break;
	case 1:
		player1Score += amount;
		break;
	case 2:
		player2Score += amount;
		break;
	case 3:
		player3Score += amount;
		break;
	}

	UpdateScores();

}

void AGameManager::UpdateScores()
{
	int index = 0;
	for (auto floating : floatingScores)
	{
		switch (index)
		{
		case 0:
			floating->SetText(FText::FromString(FString::FromInt(player0Score)));
			break;
		case 1:
			floating->SetText(FText::FromString(FString::FromInt(player1Score)));
			break;
		case 2:
			floating->SetText(FText::FromString(FString::FromInt(player2Score)));
			break;
		case 3:
			floating->SetText(FText::FromString(FString::FromInt(player3Score)));
			break;

		}
		index++;
	}
}

TArray<APackageCollectionPoint*> AGameManager::GetCollectionPoints() const
{
	return packageCollectionPoints;
}

void AGameManager::OnGameEnd()
{
	//pause all input

	//total up all players score
	playerScores.Sort();
	FString WinningScore = FString::FromInt(playerScores[0]);
	//display winner
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, WinningScore);
	//back to main menu
	UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("MainMenuScene")));
}
