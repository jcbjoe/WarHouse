// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerManager.h"
#include "FloatingScore.h"
#include "PackageCollectionPoint.h"
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

	USceneComponent* base;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		APlayerManager* _playerManager;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		TArray<AFloatingScore*> floatingScores;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		TArray<APackageCollectionPoint*> packageCollectionPoints;

	void IncrementPlayerScore(int playerIndex)
	{
		switch (playerIndex)
		{
		case 0:
			player0Score++;
			break;
		case 1:
			player1Score++;
			break;
		case 2:
			player2Score++;
			break;
		case 3:
			player3Score++;
			break;
		}

		UpdateScores();
	}

	void UpdateScores()
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

private:
	int player0Score;
	int player1Score;
	int player2Score;
	int player3Score;
};
