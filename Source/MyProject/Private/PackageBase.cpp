﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CameraManager.h"
#include "PackageProgressBar.h"
#include "SettingsSave.h"
#include "WarhouseHelpers.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APackageBase::APackageBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //package does not need to tick?

	static ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("/Game/Sounds/Cardboard/DropBox.DropBox"));
	soundBase = sound.Object;

	PackageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));

	RootComponent = PackageMesh;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Assets/JoeAssets/Package/Glow.Glow"));
	meshMaterial = FoundMaterial.Object;

	progressBar = CreateDefaultSubobject<UWidgetComponent>(FName("ProgressBar"));

	progressBar->SetupAttachment(RootComponent);

	progressBar->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> progressbarWidget(TEXT("/Game/UI/PackageCollectionBar"));

	progressBar->SetWidgetClass(progressbarWidget.Class);

	progressBar->SetDrawSize(FVector2D(200, 30));

	progressBar->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void APackageBase::InitialisePackage(FConfigPackage pi)
{
	Package = pi;

	//assign static mesh from data structure
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *Package.ModelReference));
	PackageMesh->SetStaticMesh(mesh);

	int rand = FMath::RandRange(0, Package.MaterialReferences.Num() - 1);

	UMaterial* mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.MaterialReferences[rand]));
	PackageMesh->SetMaterial(0, mat);

	PackageMesh->SetRelativeScale3D(FVector(Package.Scale));

	//assign random value from data structure value range
	PackageValue = FMath::RandRange(Package.ValueRange[0], Package.ValueRange[1]);
	//simulate physics
	PackageMesh->SetSimulatePhysics(true);
	//set rigid body collision notify (for OnHit function to work)
	PackageMesh->SetNotifyRigidBodyCollision(true);
	//assign weight
	PackageMesh->SetMassOverrideInKg(NAME_None, Package.PackageWeight, true);
	progressBar->SetWorldScale3D(FVector(1));

	PackageHealth = 100.0f;
}

// Called when the game starts or when spawned
void APackageBase::BeginPlay()
{
	Super::BeginPlay();

	PackageMesh->OnComponentHit.AddDynamic(this, &APackageBase::OnHit);

	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		volumeMultiplier = LoadedGame->SFXVolume;
	}
}

// Called every frame
void APackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (progressBar->IsVisible())
	{
		ACameraManager* cameraManager = WarhouseHelpers::GetCameraManager(GetWorld());

		auto rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cameraManager->GetMainCamera()->GetActorLocation());
		progressBar->SetWorldRotation(rot);

		auto newLoc = GetActorLocation();
		newLoc.Z += 75;
		progressBar->SetWorldLocation(newLoc);
	}

}

void APackageBase::StartHolding(AWarhousePawn* player)
{
	if (!heldBy.Contains(player)) heldBy.Add(player);
}

void APackageBase::EndHolding(AWarhousePawn* player)
{
	heldBy.Remove(player);
}

TArray<AWarhousePawn*> APackageBase::GetHeldBy() const
{
	return heldBy;
}

void APackageBase::SetProgressBarFill(float amount)
{
	Cast<UPackageProgressBar>(progressBar->GetUserWidgetObject())->progressBarFillAmount = amount;
}

void APackageBase::SetProgressBarVisability(bool visability)
{
	progressBar->SetVisibility(visability);
}

float APackageBase::GetPackageValue()
{
	return PackageValue;
}

float APackageBase::GetPackageWeight()
{
	return Package.PackageWeight;
}

float APackageBase::GetPackageHealth()
{
	return PackageHealth;
}

bool APackageBase::GetIsBeingCollected()
{
	return IsBeingCollected;
}

void APackageBase::SetIsBeingCollected(bool collected)
{
	IsBeingCollected = collected;
}

void APackageBase::SetPackageSpecial()
{
	//set special
	IsSpecial = true;
	//change its material
	UMaterial* mat;
	switch (Package.PackageSizeID)
	{
	case 1:
		mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.SpecialMaterialReferences[0])); // 0 becuase theres only one in the array?
		PackageMesh->SetMaterial(0, mat);
		break;
	case 2:
		mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.SpecialMaterialReferences[0]));
		PackageMesh->SetMaterial(0, mat);
		break;
	case 3:
		mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.SpecialMaterialReferences[0]));
		PackageMesh->SetMaterial(0, mat);
		break;
	case 4:
		mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.SpecialMaterialReferences[0]));
		PackageMesh->SetMaterial(0, mat);
		break;
	default:
		mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.SpecialMaterialReferences[0]));
		PackageMesh->SetMaterial(0, mat);
		break;
	}

}

FConfigPackage APackageBase::GetPackageDetails()
{
	return Package;
}

bool APackageBase::GetIsSpecial()
{
	return IsSpecial;
}

void APackageBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!canRegisterHit) return;
	canRegisterHit = false;
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		auto velocity = this->GetVelocity().Size();

		if (velocity > 105)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundBase, GetActorLocation(), packageDropSoundMultiplier * volumeMultiplier);

			if (PackageHealth > 0.0f && !IsBeingCollected)
				PackageHealth -= 1.f;

			if (PackageHealth < 0.0f)
				PackageHealth = 0.0f;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(timer, this, &APackageBase::AllowHit, 0.5f, false);
}

void APackageBase::AllowHit()
{
	canRegisterHit = true;
}
