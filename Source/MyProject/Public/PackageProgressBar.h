// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageProgressBar.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UPackageProgressBar : public UUserWidget
{
	GENERATED_BODY()

	
public:

	float progressBarFillAmount = 0;

	UFUNCTION(BlueprintPure)
	float GetProgressBarFillAmount()
	{
		return progressBarFillAmount;
	}
};
