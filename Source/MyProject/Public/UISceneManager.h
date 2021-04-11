// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "UISceneManager.generated.h"

UCLASS()
class MYPROJECT_API AUISceneManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUISceneManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> splashScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> mainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> playerSelect;
	

private:
	UUserWidget* currentLoadedWidget;

	void SetupPlayerControllersForUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadWidget(TSubclassOf<UUserWidget> widget);

	void UnloadCurrentWidget();
	
	UFUNCTION(BlueprintCallable)
	void ChangeActiveWidget(FString widgetName);
};
