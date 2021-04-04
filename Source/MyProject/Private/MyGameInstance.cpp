// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::addPlayer(int32 controllerId, EPlayerColours colour)
{
	playerInfo.Add({ controllerId, colour });
}