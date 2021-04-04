// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController.h"

#include "PlayerSelectBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "PlayerSelect")
	{
		TArray<UUserWidget*> widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, UPlayerSelectBase::StaticClass());

		UPlayerSelectBase* playerSelect = reinterpret_cast<UPlayerSelectBase*>(widgets[0]);

		if (playerSelect == nullptr) return;

		int32 controllerId = UGameplayStatics::GetPlayerControllerID(this);

		if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Left))
		{
			playerSelect->LeftPressed(controllerId);
		}

		if (WasInputKeyJustPressed(EKeys::Gamepad_DPad_Right))
		{
			playerSelect->RightPressed(controllerId);
		}

		if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Bottom))
		{
			playerSelect->SelectPressed(controllerId);
		}

		if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
		{
			playerSelect->UnSelectPressed(controllerId);
		}

		if (WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
		{
			playerSelect->StartPressed(controllerId);
		}
	}
}