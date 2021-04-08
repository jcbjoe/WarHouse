// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingScore.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFloatingScore::AFloatingScore()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	RootComponent = base;

	text = CreateDefaultSubobject<UTextRenderComponent>(FName("Text"));

	text->SetupAttachment(RootComponent);


	//text->SetText(FText::FromString(FString::FromInt(0)));
	FString LocalCurrencyCode = UKismetSystemLibrary::GetLocalCurrencyCode();
	text->SetText(FText::AsCurrencyBase(0.00, LocalCurrencyCode));
}

// Called when the game starts or when spawned
void AFloatingScore::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloatingScore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingScore::SetText(FText textToSet)
{
	text->SetText(textToSet);
}
