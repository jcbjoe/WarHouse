// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "WarhouseHelpers.h"
#include "PackageSpawnActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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

	LocalCurrencyCode = UKismetSystemLibrary::GetLocalCurrencyCode();
	auto instance = WarhouseHelpers::GetGameInstance(GetWorld());

	for (auto shutter : shutters)
	{
		shutter->Close();
	}

	for (auto floatingScore : floatingScores)
	{
		floatingScore->SetActorHiddenInGame(true);
	}

	for (auto PackageCollectionPoint : packageCollectionPoints)
	{
		PackageCollectionPoint->SetActorHiddenInGame(true);
	}

	if (instance->playerInfo.Num() == 0)
	{
		//add player scores to array
		playerScores.Add(player0Score);
		playerScores.Add(player1Score);
		playerScores.Add(player2Score);
		playerScores.Add(player3Score);

		for (auto shutter : shutters)
		{
			shutter->Open();
		}

		for (auto floatingScore : floatingScores)
		{
			floatingScore->SetHidden(false);
		}

		for (auto PackageCollectionPoint : packageCollectionPoints)
		{
			PackageCollectionPoint->SetHidden(false);
		}
	}
	else {

		for (int i = 0; i < instance->playerInfo.Num(); ++i)
		{
			switch (instance->playerInfo[i].controllerId)
			{
			case 0:
				playerScores.Add(player0Score);
				break;
			case 1:
				playerScores.Add(player1Score);
				break;
			case 2:
				playerScores.Add(player2Score);
				break;
			case 3:
				playerScores.Add(player3Score);
				break;
			}

			packageCollectionPoints[i]->SetActorHiddenInGame(false);
			floatingScores[i]->SetActorHiddenInGame(false);
			shutters[i]->Open();
			shutters[i]->SetColour(instance->playerInfo[i].colour);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AGameManager::ActivateForklift, ForkliftTimer, false);
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AGameManager::OnGameEnd, GameTimer, false);

	WarhouseHelpers::GetPlayerManager(GetWorld())->SpawnPlayers();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//decrease timer over delta time
	GameTimer -= (1 * DeltaTime);
	if (GameTimer < 0)
		GameTimer = 0;
	//set clock timer text
	ClockTimerText->SetTime(GameTimer);

}

void AGameManager::IncrementPlayerScore(int playerIndex, float amount)
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
			floating->SetText(FText::AsCurrencyBase(player0Score * 10, LocalCurrencyCode));
			break;
		case 1:
			floating->SetText(FText::AsCurrencyBase(player1Score * 10, LocalCurrencyCode));
			break;
		case 2:
			floating->SetText(FText::AsCurrencyBase(player2Score * 10, LocalCurrencyCode));
			break;
		case 3:
			floating->SetText(FText::AsCurrencyBase(player3Score * 10, LocalCurrencyCode));
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
	//needs implementing
	//total up all players score
	playerScores.Sort();
	FString WinningScore = FString::FromInt(playerScores[0]);
	//display winner - debugging only
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, WinningScore);
	//back to main menu after a delay
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AGameManager::ReturnToMainMenu, DelayTimer, false);
}

void AGameManager::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("UIScene")));
}

void AGameManager::ActivateForklift()
{
	Forklift->GetReadyToDeliver();
}
