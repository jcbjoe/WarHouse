// Fill out your copyright notice in the Description page of Project Settings.


#include "WarehousePackage.h"

// Sets default values
AWarehousePackage::AWarehousePackage()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create a static mesh component
	cubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Load the Cube mesh
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	// Set the component's mesh
	cubeMeshComponent->SetStaticMesh(cubeMesh);

	cubeMeshComponent->SetSimulatePhysics(true);

	// Set as root component
	RootComponent = cubeMeshComponent;


	collisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	collisionMesh->SetRelativeLocation({ 0.0,0.0,0.0 });
	collisionMesh->SetBoxExtent({ 50.0,50.0,50.0 });

	collisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AWarehousePackage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarehousePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWarehousePackage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

