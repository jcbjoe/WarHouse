// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::addPlayer(int32 controllerId, EPlayerColours colour)
{
	FPlayerInfo playerInfoInd = FPlayerInfo();
	playerInfoInd.controllerId = controllerId;
	playerInfoInd.colour = colour;
	playerInfo.Add(playerInfoInd);
}

void UMyGameInstance::AddPlayerScore(int playerId, float value, float damage)
{
	for(FPlayerInfo& playerInfoIndividual: playerInfo)
	{
		if (playerInfoIndividual.controllerId == playerId) {
			playerInfoIndividual.packageDamages += damage;
			playerInfoIndividual.packageValue += value;
			playerInfoIndividual.packagesDelivered++;
		}
	}
}

bool UMyGameInstance::IsPlayerConnected(int32 controllerId)
{
	for (FPlayerInfo& playerInfoIndividual : playerInfo)
	{
		if (playerInfoIndividual.controllerId == controllerId) {
			return true;
		}
	}

	return false;
}

float UMyGameInstance::GetPlayerOverallTotal(int32 controllerId)
{
	return GetPlayerInfo(controllerId).packageValue - GetPlayerInfo(controllerId).packageDamages;
}

int UMyGameInstance::GetPlayerPackagesDelivered(int32 controllerId)
{
	return GetPlayerInfo(controllerId).packagesDelivered;
}

float UMyGameInstance::GetPlayerPackagesValue(int32 controllerId)
{
	return GetPlayerInfo(controllerId).packageValue;
}

float UMyGameInstance::GetPlayerPackagesDamageValue(int32 controllerId)
{
	return GetPlayerInfo(controllerId).packageDamages;
}

FPlayerInfo UMyGameInstance::GetPlayerInfo(int32 controllerId)
{
	for (FPlayerInfo playerInfoIndividual : playerInfo)
	{
		if (playerInfoIndividual.controllerId == controllerId) {
			return playerInfoIndividual;
		}
	}

	throw std::exception("PlayerId not found!");
}

EPlayerColours UMyGameInstance::GetPlayerColour(int32 controllerId)
{
	return GetPlayerInfo(controllerId).colour;
}