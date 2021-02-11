// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
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
};
