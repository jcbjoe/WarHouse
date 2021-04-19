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
	
	InitGame();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (initialised) {
		if (playingIntro)
		{
			if (playingBillboard)
			{
				ACameraManager* camManager = WarhouseHelpers::GetCameraManager(GetWorld());
				camManager->SwitchCamera(camManager->GetBillboardCamera());

				introTimer += DeltaTime;

				if (introTimer > CameraSwitchTimer)
				{
					introTimer = 0.0;
					playingBillboard = false;
				}
			}
			else
			{
				if (playerIdsToIntro.Num() == 0) {
					playingIntro = false;
					GameStart();
				}
				else {

					ACameraManager* camManager = WarhouseHelpers::GetCameraManager(GetWorld());
					camManager->SwitchCamera(camManager->GetBayCamera(playerIdsToIntro[0]));

					introTimer += DeltaTime;

					if (introTimer > CameraSwitchTimer)
					{
						playerIdsToIntro.RemoveAt(0);
						introTimer = 0.0;
					}
				}
			}
		}
		else {
			if (!gameEnded) {
				//decrease timer over delta time
				GameTimer -= DeltaTime;
				if (GameTimer <= 0)
				{
					GameTimer = 0;
					OnGameEnd();
				}
				//set clock timer text
				ClockTimerText->SetTime(GameTimer);
			}
		}
	}

}

void AGameManager::AddToPlayerScore(int playerIndex, float packageValue, float damageValue)
{
	WarhouseHelpers::GetGameInstance(GetWorld())->AddPlayerScore(playerIndex, packageValue, damageValue);

	switch (playerIndex)
	{
	case 0:
		player0Score += packageValue;
		break;
	case 1:
		player1Score += packageValue;
		break;
	case 2:
		player2Score += packageValue;
		break;
	case 3:
		player3Score += packageValue;
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
			floating->SetText(FText::AsCurrencyBase(player0Score * 100, LocalCurrencyCode));
			break;
		case 1:
			floating->SetText(FText::AsCurrencyBase(player1Score * 100, LocalCurrencyCode));
			break;
		case 2:
			floating->SetText(FText::AsCurrencyBase(player2Score * 100, LocalCurrencyCode));
			break;
		case 3:
			floating->SetText(FText::AsCurrencyBase(player3Score * 100, LocalCurrencyCode));
			break;

		}
		index++;
	}
}

TArray<APackageCollectionPoint*> AGameManager::GetCollectionPoints() const
{
	return packageCollectionPoints;
}

void AGameManager::InitGame()
{
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
				numOfPlayers++;
				break;
			case 1:
				playerScores.Add(player1Score);
				numOfPlayers++;
				break;
			case 2:
				playerScores.Add(player2Score);
				numOfPlayers++;
				break;
			case 3:
				playerScores.Add(player3Score);
				numOfPlayers++;
				break;
			}

			packageCollectionPoints[i]->SetActorHiddenInGame(false);
			floatingScores[i]->SetActorHiddenInGame(false);
			shutters[i]->Open();
			shutters[i]->SetColour(instance->playerInfo[i].colour);
		}
	}

	////call intro function
	PlayIntro();

	initialised = true;
}

void AGameManager::OnGameEnd()
{
	gameEnded = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("WinScene")));
}

void AGameManager::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("UIScene")));
}

void AGameManager::ActivateForklift()
{
	Forklift->GetReadyToDeliver();
}

void AGameManager::PlayIntro()
{

	for (auto player : WarhouseHelpers::GetGameInstance(GetWorld())->playerInfo)
	{
		playerIdsToIntro.Add(player.controllerId);
	}

	introTimer = 0.0;
	playingBillboard = true;
	playingIntro = true;
}

void AGameManager::GameStart()
{
	ACameraManager* camManager = WarhouseHelpers::GetCameraManager(GetWorld());
	camManager->SwitchCamera(camManager->GetMainCamera());

	InitSpawnPlayers();

	GetWorld()->GetTimerManager().SetTimer(ForkliftTimerHandle, this, &AGameManager::ActivateForklift, ForkliftTimer, false);

}

void AGameManager::InitSpawnPlayers()
{
	WarhouseHelpers::GetPlayerManager(GetWorld())->SpawnPlayers();
}