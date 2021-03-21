// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CameraManager.h"
#include "PackageProgressBar.h"
#include "WarhouseHelpers.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APackageBase::APackageBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //package does not need to tick?
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

	UMaterial* mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *Package.MaterialReference));
	PackageMesh->SetMaterial(0, mat);

	PackageMesh->SetRelativeScale3D(FVector(Package.Scale));
	
	//assign random value from data structure value range
	PackageValue = FMath::RandRange(Package.ValueRange[0], Package.ValueRange[1]);
	//assign weight
	PackageMesh->SetMassScale(NAME_None, Package.PackageWeight);
	PackageMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void APackageBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (progressBar->IsVisible())
	{
		ACameraManager* cameraManager = WarhouseHelpers::GetCameraManager(GetWorld());

		auto rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cameraManager->GetCamera()->GetActorLocation());
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
	reinterpret_cast<UPackageProgressBar*>(progressBar->GetUserWidgetObject())->progressBarFillAmount = amount;
}

void APackageBase::SetProgressBarVisability(bool visability)
{
	progressBar->SetVisibility(visability);
}

int APackageBase::GetPackageValue()
{
	return PackageValue;
}