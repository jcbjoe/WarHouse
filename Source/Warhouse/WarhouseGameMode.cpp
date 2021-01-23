// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarhouseGameMode.h"
#include "WarhousePawn.h"

AWarhouseGameMode::AWarhouseGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AWarhousePawn::StaticClass();
}

