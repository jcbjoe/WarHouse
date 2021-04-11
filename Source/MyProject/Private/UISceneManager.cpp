// Fill out your copyright notice in the Description page of Project Settings.

#include "WarhouseHelpers.h"
#include "UISceneManager.h"

// Sets default values
AUISceneManager::AUISceneManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUISceneManager::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerControllersForUI();

	if (WarhouseHelpers::GetGameInstance(GetWorldSettings())->hasSplashRan)
	{
		LoadWidget(mainMenu);
	}
	else
	{
		WarhouseHelpers::GetGameInstance(GetWorldSettings())->hasSplashRan = true;
		LoadWidget(splashScreen);
	}

}

// Called every frame
void AUISceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUISceneManager::LoadWidget(TSubclassOf<UUserWidget> widget)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	currentLoadedWidget = UUserWidget::CreateWidgetInstance(*playerController, widget, FName(widget->GetName()));

	currentLoadedWidget->AddToViewport();
}

void AUISceneManager::UnloadCurrentWidget()
{
	currentLoadedWidget->RemoveFromViewport();
	currentLoadedWidget->Destruct();

	currentLoadedWidget = nullptr;
}

void AUISceneManager::SetupPlayerControllersForUI()
{
	TArray<AActor*> controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), controllers);

	for (AActor* controller : controllers)
	{
		APlayerController* player = reinterpret_cast<APlayerController*>(controller);
		player->K2_DestroyActor();
	}

	for (int i = 0; i < 4; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}
}

void AUISceneManager::ChangeActiveWidget(FString widgetName) {

	FString oldName = FString("");
	if (currentLoadedWidget != nullptr) {
		oldName = currentLoadedWidget->GetName();
		UnloadCurrentWidget();
		
	}

	if (widgetName == "playerselect")
	{
		if (oldName == playerSelect->GetName()) return;
		LoadWidget(playerSelect);
	}

	if (widgetName == "mainmenu")
	{
		if (oldName == mainMenu->GetName()) return;
		LoadWidget(mainMenu);
	}


	if (widgetName == "splash")
	{
		if (oldName == splashScreen->GetName()) return;
		LoadWidget(splashScreen);
	}

}