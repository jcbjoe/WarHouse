// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseHelpers.h"

#include "Kismet/GameplayStatics.h"

WarhouseHelpers::WarhouseHelpers()
{
}

WarhouseHelpers::~WarhouseHelpers()
{
}

AGameManager* WarhouseHelpers::GetGameManager(UObject* world)
{
	return reinterpret_cast<AGameManager*>(UGameplayStatics::GetActorOfClass(world, AGameManager::StaticClass()));
}

APlayerManager* WarhouseHelpers::GetPlayerManager(UObject* world)
{
	return reinterpret_cast<APlayerManager*>(UGameplayStatics::GetActorOfClass(world, APlayerManager::StaticClass()));
}

APackageManager* WarhouseHelpers::GetPackageManager(UObject* world)
{
	return reinterpret_cast<APackageManager*>(UGameplayStatics::GetActorOfClass(world, APackageManager::StaticClass()));
}

ACameraManager* WarhouseHelpers::GetCameraManager(UObject* world)
{
	return reinterpret_cast<ACameraManager*>(UGameplayStatics::GetActorOfClass(world, ACameraManager::StaticClass()));
}