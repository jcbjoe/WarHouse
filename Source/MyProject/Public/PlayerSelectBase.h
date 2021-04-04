// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSelectBase.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UPlayerSelectBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SelectPressed(int32 playerId);

	UFUNCTION(BlueprintImplementableEvent)
		void UnSelectPressed(int32 playerId);

	UFUNCTION(BlueprintImplementableEvent)
		void LeftPressed(int32 playerId);

	UFUNCTION(BlueprintImplementableEvent)
		void RightPressed(int32 playerId);

	UFUNCTION(BlueprintImplementableEvent)
		void StartPressed(int32 playerId);
};
