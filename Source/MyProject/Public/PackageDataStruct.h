// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "PackageDataStruct.generated.h"

USTRUCT()
struct FPackageDataStructure
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Data Structure")
		FString PackageName;
	UPROPERTY(EditAnywhere, Category = "Data Structure")
		FString ModelReference;
	UPROPERTY(EditAnywhere, Category = "Data Structure")
		TArray<int> ValueRange;
	UPROPERTY(EditAnywhere, Category = "Data Structure")
		int Rarity;
	UPROPERTY(EditAnywhere, Category = "Data Structure")
		float PackageWeight;
};

USTRUCT()
struct FConfigPackage
{

	GENERATED_BODY()

		UPROPERTY()
		FString PackageName;

	UPROPERTY()
		FString ModelReference;

	UPROPERTY()
		TArray<int> ValueRange;

	UPROPERTY()
		int32 Rarity;

	UPROPERTY()
		int32 PackageWeight;

	FConfigPackage() {};

};

USTRUCT()
struct FConfig
{

	GENERATED_BODY()

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
