// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController.h"

#include "UserInterfaceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TArray<UUserWidget*> widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, UUserInterfaceBase::StaticClass());

	if (widgets.Num() == 0) return;

	const int32 controllerId = UGameplayStatics::GetPlayerControllerID(this);

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Left))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->LeftPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Right))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->RightPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Bottom))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->SelectPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->UnSelectPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->StartPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->UpPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down))
	{
		for (auto widget : widgets)
		{
			UUserInterfaceBase* userInterface = reinterpret_cast<UUserInterfaceBase*>(widget);
			userInterface->DownPressed(controllerId);
		}
	}
}