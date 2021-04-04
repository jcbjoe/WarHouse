// Copyright Epic Games, Inc. All Rights Reserved.
#include "MyPlayerController.h"
#include "MyGameStateBase.h"
#include "MyProjectGameMode.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = nullptr;

	PlayerControllerClass = AMyPlayerController::StaticClass();

	GameStateClass = AMyGameStateBase::StaticClass();
}
