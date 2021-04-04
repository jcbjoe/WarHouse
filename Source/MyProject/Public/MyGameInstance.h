// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

struct playerInfo
{
	int32 controllerId;
	int32 textureId;
};

/**
 *
 */
UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	TArray<playerInfo> playerInfo;

	UFUNCTION(BlueprintCallable)
		void addPlayer(int32 controllerId, int32 textureIndex);
};
