// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "CameraManager.h"
#include "GameManager.h"
#include "MyGameInstance.h"
#include "PackageManager.h"

/**
 *
 */
class MYPROJECT_API WarhouseHelpers
{
public:
	WarhouseHelpers();
	~WarhouseHelpers();

	static AGameManager* GetGameManager(UObject* world);

	static APlayerManager* GetPlayerManager(UObject* world);

	static APackageManager* GetPackageManager(UObject* world);

	static ACameraManager* GetCameraManager(UObject* world);

	static UMyGameInstance* GetGameInstance(UObject* world);
};
