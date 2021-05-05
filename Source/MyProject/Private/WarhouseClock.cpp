// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseClock.h"
#include "Misc/Timespan.h"

// Sets default values
AWarhouseClock::AWarhouseClock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//set root
	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	RootComponent = base;
	//set up mesh
	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	ClockMesh->SetupAttachment(RootComponent);
	//set up text component
	TimeText = CreateDefaultSubobject< UTextRenderComponent>(TEXT("TimeText"));
	TimeText->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWarhouseClock::BeginPlay()
{
	Super::BeginPlay();

}

void AWarhouseClock::SetTime(int time)
{
	//format textToSet into time format
	int mins = (time / 60) % 60;
	int secs = time % 60;
	FString TimeRemaining;
	if (secs < 10)
		TimeRemaining = FString::FromInt(mins) + ":0" + FString::FromInt(secs);
	else
		TimeRemaining = FString::FromInt(mins) + ":" + FString::FromInt(secs);

	TimeText->SetText(TimeRemaining);
}

// Called every frame
void AWarhouseClock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
