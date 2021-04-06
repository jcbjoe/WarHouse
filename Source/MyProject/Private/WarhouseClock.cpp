// Fill out your copyright notice in the Description page of Project Settings.


#include "WarhouseClock.h"

// Sets default values
AWarhouseClock::AWarhouseClock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWarhouseClock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarhouseClock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

