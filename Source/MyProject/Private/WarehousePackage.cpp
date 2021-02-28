// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "WarehousePackage.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWarehousePackage::AWarehousePackage()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create a static mesh component
	cubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Load the Cube mesh
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	UMaterial* glowMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/JoeAssets/Package/Glow.Glow'")).Object;

	cubeMeshComponent->SetStaticMesh(cubeMesh);
	cubeMeshComponent->SetMaterial(0, glowMaterial);

	cubeMeshComponent->SetRelativeScale3D({ 0.5f,0.5f,0.5f });

	cubeMeshComponent->SetSimulatePhysics(true);

	// Set as root component
	RootComponent = cubeMeshComponent;


	collisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	collisionMesh->SetBoxExtent({ 100.0,100.0,100.0 });

	collisionMesh->SetupAttachment(RootComponent);


	progressBar = CreateDefaultSubobject<UWidgetComponent>(FName("ProgressBar"));

	progressBar->SetupAttachment(RootComponent);

	progressBar->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> progressbarWidget(TEXT("/Game/UI/PackageCollectionBar"));

	progressBar->SetWidgetClass(progressbarWidget.Class);

	progressBar->SetDrawSize(FVector2D(200, 30));

	progressBar->SetVisibility(false);

}

// Called when the game starts or when spawned
void AWarehousePackage::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), cameras);

	cam = (ACameraActor*)cameras[0];
}

// Called every frame
void AWarehousePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(progressBar->IsVisible())
	{
		auto rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cam->GetActorLocation());
		progressBar->SetWorldRotation(rot);

		auto newLoc = GetActorLocation();
		newLoc.Z += 75;
		progressBar->SetWorldLocation(newLoc);
	}

}