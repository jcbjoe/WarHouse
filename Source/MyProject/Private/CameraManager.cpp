// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "PlayerManager.h"
#include "WarhouseHelpers.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

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
	SwitchCamera(BillboardCamera);
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerManager* PlayerManager = WarhouseHelpers::GetPlayerManager(GetWorld());

	auto players = PlayerManager->GetPlayers();

	FVector middlePos;
	for (int i = 0; i < players.Num(); ++i)
	{
		if (players[i]->IsDead()) continue;
		if (i == 0) middlePos = players[0]->GetActorLocation();
		else middlePos = (middlePos + players[i]->GetActorLocation()) / 2;
	}

	const FRotator loc = UKismetMathLibrary::FindLookAtRotation(MainCamera->GetActorLocation(), middlePos);

	const FRotator lerped = FMath::Lerp(MainCamera->GetActorRotation(), loc, DeltaTime);

	MainCamera->SetActorRotation(lerped);

	float maxDistance = 0;

	for (int i = 0; i < players.Num(); ++i)
	{
		if (players[i]->IsDead()) continue;
		const float dist = FVector::Distance(players[i]->GetActorLocation(), middlePos);
		if (maxDistance == 0) maxDistance = dist;
		else
		{
			if (dist > maxDistance) maxDistance = dist;
		}

	}

	const int minDist = 57;
	const int maxDist = 2000;

	const float zoom = 90 - UKismetMathLibrary::MapRangeClamped(maxDistance, minDist, maxDist, 45, 90);

	const float lerpedZooom = FMath::Lerp(MainCamera->GetCameraComponent()->FieldOfView, 90 - zoom, DeltaTime);

	MainCamera->GetCameraComponent()->FieldOfView = lerpedZooom;

}

void ACameraManager::SwitchCamera(ACameraActor* camera)
{
	auto p = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	p->SetViewTargetWithBlend(camera);
}
