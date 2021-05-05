// Fill out your copyright notice in the Description page of Project Settings.
#include "UISceneManager.h"

#include "UserInterfaceBase.h"
#include "WarhouseHelpers.h"


// Sets default values
AUISceneManager::AUISceneManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AUISceneManager::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerControllersForUI();

	if (WarhouseHelpers::GetGameInstance(GetWorldSettings())->hasSplashRan)
	{
		ChangeActiveWidget("mainmenu");
	}
	else
	{
		WarhouseHelpers::GetGameInstance(GetWorldSettings())->hasSplashRan = true;
		ChangeActiveWidget("splashscreen");
	}

}

// Called every frame
void AUISceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUISceneManager::UnloadCurrentWidget()
{
	if (currentWidget != nullptr)
	{
		Cast<UUserInterfaceBase>(currentWidget)->canRecieveInput = false;
		currentWidget->SetVisibility(ESlateVisibility::Hidden);
		currentWidget->RemoveFromParent();
		currentWidget = nullptr;
	}
}

void AUISceneManager::SetupPlayerControllersForUI()
{
	//create an array to hold all player controllers
	TArray<AActor*> controllers;
	//get all player controllers and store them in the array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), controllers);

	for (AActor* controller : controllers)
	{
		APlayerController* player = Cast<APlayerController>(controller);
		player->K2_DestroyActor();
	}

	for (int i = 0; i < 4; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}
}

void AUISceneManager::ChangeActiveWidget(FName widgetName) {

	UnloadCurrentWidget();

	check(widgets.Contains(widgetName));

	auto widget = widgets[widgetName];
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto newWidget = UUserWidget::CreateWidgetInstance(*playerController, widget, FName(widget->GetName()));

	Cast<UUserInterfaceBase>(newWidget)->canRecieveInput = true;

	currentWidget = newWidget;

	newWidget->AddToViewport();
}
