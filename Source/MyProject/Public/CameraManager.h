// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

UCLASS()
class MYPROJECT_API ACameraManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACameraActor* GetMainCamera();

	ACameraActor* GetBillboardCamera();

	ACameraActor* GetWinScreenCamera();

	ACameraActor* GetBayCamera(int bay);

	TArray<ACameraActor*> GetBayCams();

	UFUNCTION()
		void SwitchCamera(ACameraActor* camera, int blendTime = 0);

	void SetMainCameraFollowingPlayers(bool isFollowing);

	ACameraActor* GetCurrentCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* MainCamera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* BillboardCamera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay1Camera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay2Camera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay3Camera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay4Camera;
	
	UPROPERTY(Category = Cameras, EditAnywhere, BlueprintReadWrite)
		ACameraActor* WinScreenCamera;

private:
	bool isCameraFollowingPlayers = true;

	ACameraActor* currentCamera = nullptr;

	FVector lastGoodPos = {0,0,0};

	TArray<ACameraActor*> BayCameras;
};
