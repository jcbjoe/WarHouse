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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* MainCamera;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* BillboardCamera;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay1Camera;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay2Camera;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay3Camera;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		ACameraActor* Bay4Camera;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACameraActor* GetMainCamera() const
	{
		return MainCamera;
	}

	ACameraActor* GetBillboardCamera() const
	{
		return BillboardCamera;
	}

	ACameraActor* GetBayCamera(int bay) const;

	TArray<ACameraActor*> BayCameras;

	TArray<ACameraActor*> GetBayCams()
	{
		return BayCameras;
	}

	UFUNCTION()
		void SwitchCamera(ACameraActor* camera);
};
