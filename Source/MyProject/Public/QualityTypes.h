// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QualityTypes.generated.h"

/**
 *
 */
UENUM(BlueprintType) //"BlueprintType" is essential to include
enum class EQualityTypes : uint8
{
	Low,
	Medium,
	High,
	Ultra
};