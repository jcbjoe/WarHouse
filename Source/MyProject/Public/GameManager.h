// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerManager.h"
#include "FloatingScore.h"
#include "PackageCollectionPoint.h"
#include "PackageManager.h"
#include "Shutter.h"
#include "GameManager.generated.h"

UCLASS()
class MYPROJECT_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		TArray<AFloatingScore*> floatingScores;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		TArray<APackageCollectionPoint*> packageCollectionPoints;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		TArray<AShutter*> shutters;

	USceneComponent* base;

	int player0Score;
	int player1Score;
	int player2Score;
	int player3Score;

	UPROPERTY(VisibleAnywhere)
		TArray<int> playerScores;
	UPROPERTY(EditAnywhere)
		float GameTimer;
	UPROPERTY(EditAnywhere)
		float DelayTimer;

	//Handle to manage the game timer
	FTimerHandle GameTimerHandle;
	//Handle to manage delays
	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere)
		AFloatingScore* GameTimerText;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void IncrementPlayerScore(int playerIndex, int amount = 1);

	void UpdateScores();

	TArray<APackageCollectionPoint*> GetCollectionPoints() const;

	APlayerManager* GetPlayerManager() const;
	APackageManager* GetPackageManager() const;

	UFUNCTION()
		void OnGameEnd();
	UFUNCTION()
		void ReturnToMainMenu(); //could move this into the helper class?
};
