﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "PackageDataStruct.generated.h"

USTRUCT()
struct FConfigPackage
{

	GENERATED_BODY()

		UPROPERTY()
		FString PackageName;

	UPROPERTY()
		FString ModelReference;

	UPROPERTY()
		FString MaterialReference;
	
	UPROPERTY()
		float Scale;

	UPROPERTY()
		TArray<int> ValueRange;

	UPROPERTY()
		int32 Rarity;

	UPROPERTY()
		float PackageWeight;

	FConfigPackage() {};

};

USTRUCT()
struct FConfig
{

	GENERATED_BODY()

		UPROPERTY()
		float GameTimer;
	
		UPROPERTY()
		TArray<FConfigPackage> packages;

	FConfig() {};

	FConfig(FString _json_) {
		FConfig _tmpConfig;

		FJsonObjectConverter::JsonObjectStringToUStruct<FConfig>(
			_json_,
			&_tmpConfig,
			0, 0);

		packages = _tmpConfig.packages;
	}

};
