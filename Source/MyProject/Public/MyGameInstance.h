// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerColours.h"
#include "MyGameInstance.generated.h"

USTRUCT(Blueprintable)
struct FPlayerInfo
{
	GENERATED_BODY()
	
	int32 controllerId;
	EPlayerColours colour;
	int packagesDelivered = 0;
	float packageValue = 0.0;
	float packageDamages = 0.0;
};

/**
 *
 */
UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	TArray<FPlayerInfo> playerInfo;

	void AddPlayerScore(int32 controllerId, float value, float damage);

	UFUNCTION(BlueprintPure)
	bool IsPlayerConnected(int32 controllerId);

	UFUNCTION(BlueprintCallable)
		void addPlayer(int32 controllerId, EPlayerColours colour);

	UFUNCTION(BlueprintPure)
		float GetPlayerOverallTotal(int32 controllerId);

	UFUNCTION(BlueprintPure)
		int GetPlayerPackagesDelivered(int32 controllerId);

	UFUNCTION(BlueprintPure)
		float GetPlayerPackagesValue(int32 controllerId);

	UFUNCTION(BlueprintPure)
		float GetPlayerPackagesDamageValue(int32 controllerId);

	UFUNCTION(BlueprintPure)
		FPlayerInfo GetPlayerInfo(int32 controllerId);

	UFUNCTION(BlueprintPure)
		EPlayerColours GetPlayerColour(int32 controllerId);

	UFUNCTION(BlueprintCallable)
	void WipePlayerList();
	
	bool hasSplashRan = false;
};
