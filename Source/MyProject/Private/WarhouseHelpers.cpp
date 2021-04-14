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
	return Cast<AGameManager>(UGameplayStatics::GetActorOfClass(world, AGameManager::StaticClass()));
}

APlayerManager* WarhouseHelpers::GetPlayerManager(UObject* world)
{
	return Cast<APlayerManager>(UGameplayStatics::GetActorOfClass(world, APlayerManager::StaticClass()));
}

APackageManager* WarhouseHelpers::GetPackageManager(UObject* world)
{
	return Cast<APackageManager>(UGameplayStatics::GetActorOfClass(world, APackageManager::StaticClass()));
}

ACameraManager* WarhouseHelpers::GetCameraManager(UObject* world)
{
	return Cast<ACameraManager>(UGameplayStatics::GetActorOfClass(world, ACameraManager::StaticClass()));
}

UMyGameInstance* WarhouseHelpers::GetGameInstance(UObject* world)
{
	return Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(world));
}