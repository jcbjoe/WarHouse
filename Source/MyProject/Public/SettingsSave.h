// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QualityTypes.h"
#include "SettingsSave.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API USettingsSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float MusicVolume;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float SFXVolume;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool VibrationEnabled;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes GraphicsQuality;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes Shadows;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes AntiAliasing;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes PostProcessing;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes ViewDistance;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes TextureQuality;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EQualityTypes Effects;
};
