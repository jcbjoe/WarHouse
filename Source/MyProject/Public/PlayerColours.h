// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PlayerColours.generated.h"

UENUM(BlueprintType) //"BlueprintType" is essential to include
enum class EPlayerColours : uint8
{
	White,
	Red,
	Blue,
	Yellow,
	glasses,
	smiley,
	tux
};

/**
 *
 */
UCLASS()
class MYPROJECT_API UPlayerColours : public UUserDefinedEnum
{
	GENERATED_BODY()

};
