// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::addPlayer(int32 controllerId, int32 textureIndex)
{
	playerInfo.Add({ controllerId, textureIndex });
}