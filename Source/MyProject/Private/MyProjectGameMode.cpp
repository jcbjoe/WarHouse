// Copyright Epic Games, Inc. All Rights Reserved.
#include "MyProjectGameMode.h"
#include "MyPlayerController.h"
#include "MyGameStateBase.h"


AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = nullptr;

	PlayerControllerClass = AMyPlayerController::StaticClass();

	GameStateClass = AMyGameStateBase::StaticClass();
}
