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
	
	BayCameras.Add(Bay1Camera);
	BayCameras.Add(Bay2Camera);
	BayCameras.Add(Bay3Camera);
	BayCameras.Add(Bay4Camera);
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

	//--- If the camera is following the player, centre on the player
	if (isCameraFollowingPlayers) {
		APlayerManager* PlayerManager = WarhouseHelpers::GetPlayerManager(GetWorld());

		TArray<AWarhousePawn*> players = PlayerManager->GetPlayers();

		bool middlePosStarted = false;
		FVector middlePos;
		//--- Loop the players and if the player is not dead then grab their position
		//--- Once an alive player is found that players position is used as the mid point
		//--- every players position after that will be averaged, finding a centre point
		for (int i = 0; i < players.Num(); ++i)
		{
			if (players[i]->IsDead()) continue;
			if (!middlePosStarted) {
				middlePos = players[i]->GetActorLocation();
				middlePosStarted = true;
			}
			else {
				middlePos = (middlePos + players[i]->GetActorLocation()) / 2;
			}
		}

		//--- If no centre point found then we use the last good one. This is used for when all players die.
		if(middlePosStarted)
		{
			lastGoodPos = middlePos;
		} else
		{
			middlePos = lastGoodPos;
		}

		//--- Lerp the camera to make it smooth and not just snap
		const FRotator loc = UKismetMathLibrary::FindLookAtRotation(MainCamera->GetActorLocation(), middlePos);
		const FRotator lerped = FMath::Lerp(MainCamera->GetActorRotation(), loc, DeltaTime);

		MainCamera->SetActorRotation(lerped);

		float maxDistance = 0;

		//--- Calculate the max distance between all the players
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

		//--- Using the max distance found either zoom in or zoom out the the camera.
		const float zoom = 90 - UKismetMathLibrary::MapRangeClamped(maxDistance, minDist, maxDist, 45, 90);

		const float lerpedZooom = FMath::Lerp(MainCamera->GetCameraComponent()->FieldOfView, 90 - zoom, DeltaTime);

		MainCamera->GetCameraComponent()->FieldOfView = lerpedZooom;
	}
}

ACameraActor* ACameraManager::GetBayCamera(int bay)
{
	ACameraActor* camera = nullptr;
	switch (bay)
	{
		case 0:
			camera = Bay1Camera;
			break;
		case 1:
			camera = Bay2Camera;
			break;
		case 2:
			camera = Bay3Camera;
			break;
		case 3:
			camera = Bay4Camera;
			break;
	}

	return camera;
}

void ACameraManager::SwitchCamera(ACameraActor* camera, int blendTime)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//--- The blendTime makes the camera lerp to the new position rather than snapping
	playerController->SetViewTargetWithBlend(camera, blendTime);

	currentCamera = camera;
}
void ACameraManager::SetMainCameraFollowingPlayers(bool isFollowing)
{
	isCameraFollowingPlayers = isFollowing;
}

ACameraActor* ACameraManager::GetCurrentCamera()
{
	return currentCamera;
}

ACameraActor* ACameraManager::GetMainCamera()
{
	return MainCamera;
}

ACameraActor* ACameraManager::GetBillboardCamera()
{
	return BillboardCamera;
}

ACameraActor* ACameraManager::GetWinScreenCamera()
{
	return WinScreenCamera;
}

TArray<ACameraActor*> ACameraManager::GetBayCams()
{
	return BayCameras;
}