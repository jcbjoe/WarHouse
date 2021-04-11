// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaceBase.h"

#include "Kismet/GameplayStatics.h"

AUISceneManager* UUserInterfaceBase::GetUISceneManager()
{
	TArray<AActor*> sceneManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUISceneManager::StaticClass(), sceneManagers);

	return reinterpret_cast<AUISceneManager*>(sceneManagers[0]);
}
