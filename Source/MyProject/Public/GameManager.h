// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerManager.h"
#include "FloatingScore.h"
#include "PackageCollectionPoint.h"
#include "PackageManager.h"
#include "Shutter.h"
#include "WarhouseClock.h"
#include "WarhouseForklift.h"
#include "Components/WidgetComponent.h"
#include "LevelSequencePlayer.h"
#include "Engine/StaticMeshActor.h"
#include "Components/AudioComponent.h"
#include "GameManager.generated.h"

UCLASS()
class MYPROJECT_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddToPlayerScore(int playerIndex, float packageValue = 0, float damageValue = 0);

	void UpdateScores();

	TArray<APackageCollectionPoint*> GetCollectionPoints() const;

	APlayerManager* GetPlayerManager() const;
	APackageManager* GetPackageManager() const;

	TArray<AShutter*> GetShutters();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//--- Components
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* base;

	UPROPERTY(EditDefaultsOnly)
		UAudioComponent* backgroundMusic;

	UPROPERTY(EditDefaultsOnly)
		UWidgetComponent* winScreen;

	//--- Editor Variables
	UPROPERTY(Category = Gameplay, EditAnywhere)
		TArray<AFloatingScore*> floatingScores;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		TArray<APackageCollectionPoint*> packageCollectionPoints;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		AWarhouseForklift* Forklift;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		TArray<AShutter*> shutters;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		AStaticMeshActor* winScreenBillboard;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		float WinBillboardUIOffset = -7.5;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		TSubclassOf<UUserWidget> winscreenWidget;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		USoundBase* backgroundMusicTrack;

	UPROPERTY(Category = Gameplay, EditAnywhere)
		AWarhouseClock* ClockTimerText;

	UPROPERTY(Category = Timers, EditAnywhere)
		float GameTimer = 120;
	UPROPERTY(Category = Timers, EditAnywhere)
		float ForkliftTimer;
	UPROPERTY(Category = Timers, EditAnywhere)
		float PlayerSwitchTimer = 7.5f; //how long on each camera

	UPROPERTY(Category = Timers, EditAnywhere)
		float InstructionsSwitchTimer = 4f; //how long on each camera

private:
	bool initialised = false;
	bool playingIntro = true;
	bool playingBillboard = false;
	float introTimer = 0.0;
	bool gameEnded = false;
	TArray<int> playerIdsToIntro;

	void GameStart();

	float player0Score;
	float player1Score;
	float player2Score;
	float player3Score;
	int numOfPlayers;

	TArray<float> playerScores;

	FTimerHandle ForkliftTimerHandle;

	void InitGame();
	void OnGameEnd();
	void ReturnToMainMenu();
	void ActivateForklift();
	void PlayIntro();
	void InitSpawnPlayers();
};
