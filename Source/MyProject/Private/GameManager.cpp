// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "MovieSceneSequencePlayer.h"
#include "WarhouseHelpers.h"
#include "SettingsSave.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//--- Setting up imports
	static ConstructorHelpers::FClassFinder<UUserWidget> winScreenWidgetObj(TEXT("/Game/UI/Menus/WinScreen/WinScreenWidget"));

	//--- Setting up root scene component
	base = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = base;

	//--- Setting up winscreen widget
	winScreen = CreateDefaultSubobject<UWidgetComponent>(FName("WinScreen"));
	winScreen->SetupAttachment(RootComponent);
	winScreen->SetWidgetSpace(EWidgetSpace::World);
	winscreenWidget = winScreenWidgetObj.Class;

	//--- Setting up background music
	backgroundMusic = CreateDefaultSubobject<UAudioComponent >(FName("BackgroundMusic"));
	backgroundMusic->SetAutoActivate(false);
	backgroundMusic->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	check(floatingScores.Num() != 0);
	check(packageCollectionPoints.Num() != 0);
	check(winScreenBillboard != nullptr);
	check(shutters.Num() != 0);
	check(Forklift != nullptr);
	check(ClockTimerText != nullptr);
	check(backgroundMusicTrack != nullptr);

	float soundVol = 0.1;

	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		soundVol = soundVol * LoadedGame->MusicVolume;
	}

	backgroundMusic->SetSound(backgroundMusicTrack);
	backgroundMusic->SetVolumeMultiplier(soundVol);
	backgroundMusic->Play();

	InitGame();
	//set clock timer text
	ClockTimerText->SetTime(GameTimer);
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

				if (introTimer > InstructionsSwitchTimer)
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

					if (camManager->GetCurrentCamera() != camManager->GetBayCamera(playerIdsToIntro[0]))
					{
						shutters[playerIdsToIntro[0]]->Open();
					}

					camManager->SwitchCamera(camManager->GetBayCamera(playerIdsToIntro[0]));

					introTimer += DeltaTime;

					if (introTimer > PlayerSwitchTimer)
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
			floating->SetText(FText::AsCurrencyBase(player0Score * 100, TEXT("GBP")));
			break;
		case 1:
			floating->SetText(FText::AsCurrencyBase(player1Score * 100, TEXT("GBP")));
			break;
		case 2:
			floating->SetText(FText::AsCurrencyBase(player2Score * 100, TEXT("GBP")));
			break;
		case 3:
			floating->SetText(FText::AsCurrencyBase(player3Score * 100, TEXT("GBP")));
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

	if (instance->GetPlayerInfo().Num() == 0)
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

		for (int i = 0; i < instance->GetPlayerInfo().Num(); ++i)
		{
			const int controllerId = instance->GetPlayerInfo()[i].controllerId;

			switch (controllerId)
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

			packageCollectionPoints[controllerId]->SetActorHiddenInGame(false);
			floatingScores[controllerId]->SetActorHiddenInGame(false);
			shutters[controllerId]->SetColour(instance->GetPlayerInfo()[i].colour);
		}
	}

	PlayIntro();

	initialised = true;
}

void AGameManager::OnGameEnd()
{
	gameEnded = true;
	winScreen->SetWidgetClass(winscreenWidget);
	winScreen->SetDrawSize(FVector2D(1920, 1080));
	FVector newpos = winScreenBillboard->GetActorLocation();
	newpos.Y = newpos.Y + WinBillboardUIOffset;
	Cast<USceneComponent>(winScreen)->SetWorldLocationAndRotation(newpos, winScreenBillboard->GetActorRotation());
	Cast<USceneComponent>(winScreen)->SetRelativeScale3D({ 1.0,0.342417 ,0.342417 });

	WarhouseHelpers::GetCameraManager(GetWorld())->SetMainCameraFollowingPlayers(false);
	WarhouseHelpers::GetCameraManager(GetWorld())->SwitchCamera(WarhouseHelpers::GetCameraManager(GetWorld())->GetWinScreenCamera(), 3);
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

	for (auto player : WarhouseHelpers::GetGameInstance(GetWorld())->GetPlayerInfo())
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

TArray<AShutter*> AGameManager::GetShutters()
{
	return shutters;
}
