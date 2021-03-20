// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "PlayerManager.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> playerManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerManager::StaticClass(), playerManagers);

	APlayerManager* PlayerManager = reinterpret_cast<APlayerManager*>(playerManagers[0]);

	auto players = PlayerManager->GetPlayers();

	FVector middlePos;
	for(int i = 0; i < players.Num(); ++i)
	{
		if (i == 0) middlePos = players[0]->GetActorLocation();
		else middlePos = (middlePos + players[i]->GetActorLocation()) / 2;
	}

	auto loc = UKismetMathLibrary::FindLookAtRotation(MainCamera->GetActorLocation(), middlePos);

	MainCamera->SetActorRotation(loc);

	float maxDistance = 0;

	for (int i = 0; i < players.Num(); ++i)
	{
		auto dist = FVector::Distance(players[0]->GetActorLocation(), middlePos);
		if (i == 0) maxDistance = dist;

		if (dist > maxDistance) maxDistance = dist;
	}

	const int minDist = 57;
	const int maxDist = 2000;

	float zoom  = 90 - UKismetMathLibrary::MapRangeClamped(maxDistance, minDist, maxDist, 45, 90);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Zoom: %f"), zoom));
	
	MainCamera->GetCameraComponent()->FieldOfView = 90 - zoom;

}

