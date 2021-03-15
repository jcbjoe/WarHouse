// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageBase.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	Package.PackageName = pi.PackageName;
	Package.ModelReference = pi.ModelReference;
	Package.ValueRange = pi.ValueRange;
	Package.Rarity = pi.Rarity;
	Package.PackageWeight = pi.PackageWeight;

	//assign static mesh from data structure
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *pi.ModelReference));
	PackageMesh->SetStaticMesh(mesh);
	//assign random value from data structure value range
	PackageValue = FMath::RandRange(pi.ValueRange[0], pi.ValueRange[1]);
	//assign weight
	PackageMesh->SetMassScale(NAME_None, pi.PackageWeight);
	PackageMesh->SetSimulatePhysics(true);
	//assign material
	PackageMesh->SetMaterial(0, meshMaterial);

	PackageMesh->SetRelativeScale3D({ 0.5f,0.5f,0.5f });
}

// Called when the game starts or when spawned
void APackageBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), cameras);

	cam = reinterpret_cast<ACameraActor*>(cameras[0]);
}

// Called every frame
void APackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (progressBar->IsVisible())
	{
		TArray<AActor*> managers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), managers);

		AGameManager* manager = reinterpret_cast<AGameManager*>(managers[0]);
		
		auto rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), manager->MainCamera->GetActorLocation());
		progressBar->SetWorldRotation(rot);

		auto newLoc = GetActorLocation();
		newLoc.Z += 75;
		progressBar->SetWorldLocation(newLoc);
	}

}
