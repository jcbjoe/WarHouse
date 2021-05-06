// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UISceneManager.h"
#include "Blueprint/UserWidget.h"
#include "UserInterfaceBase.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UUserInterfaceBase : public UUserWidget
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

	UFUNCTION(BlueprintImplementableEvent)
		void UpPressed(int32 playerId);

	UFUNCTION(BlueprintImplementableEvent)
		void DownPressed(int32 playerId);

	UFUNCTION(BlueprintCallable)
		AUISceneManager* GetUISceneManager();

	bool canRecieveInput = true;
};
