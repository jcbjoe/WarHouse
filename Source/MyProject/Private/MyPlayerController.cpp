// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController.h"

#include "UserInterfaceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AMyPlayerController::AMyPlayerController() {
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<UUserWidget*> widgetsFound;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgetsFound, UUserInterfaceBase::StaticClass(), false);

	if (widgetsFound.Num() == 0) return;

	TArray<UUserInterfaceBase*> widgets;

	for (auto widget : widgetsFound)
	{
		UUserInterfaceBase* userInterface = Cast<UUserInterfaceBase>(widget);
		if (userInterface->canRecieveInput && userInterface->IsConstructed()) widgets.Add(userInterface);
	}

	const int32 controllerId = UGameplayStatics::GetPlayerControllerID(this);

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Left) || WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Left))
	{
		for (auto widget : widgets)
		{
			widget->LeftPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Right) || WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Right))
	{
		for (auto widget : widgets)
		{
			widget->RightPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Bottom))
	{
		for (auto widget : widgets)
		{
			widget->SelectPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
	{
		for (auto widget : widgets)
		{
			widget->UnSelectPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
	{
		for (auto widget : widgets)
		{
			widget->StartPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up) || WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Up))
	{
		for (auto widget : widgets)
		{
			widget->UpPressed(controllerId);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down) || WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Down))
	{
		for (auto widget : widgets)
		{
			widget->DownPressed(controllerId);
		}
	}
}
