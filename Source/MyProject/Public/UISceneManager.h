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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UnloadCurrentWidget();

	UFUNCTION(BlueprintCallable)
		void ChangeActiveWidget(FName widgetName);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FName, TSubclassOf<UUserWidget>> widgets;
private:
	void SetupPlayerControllersForUI();
	UUserWidget* currentWidget = nullptr;
};
