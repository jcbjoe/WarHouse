// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

AMyProjectPawn::AMyProjectPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh

	PlaneMesh->BodyInstance.bLockXRotation = true;
	PlaneMesh->BodyInstance.bLockYRotation = true;
	PlaneMesh->BodyInstance.bLockZRotation = true;

	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 300.f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,40.f);
	SpringArm->SetRelativeLocation({ 0.0,0.0,50.0 });
	SpringArm->SetRelativeRotation(FQuat::MakeFromEuler({ 0.0,-20.0,0.0 }));
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;

	collisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	collisionMesh->SetRelativeLocation({ -20.0,0.0,-10.0 });
	collisionMesh->SetBoxExtent({ 60.0,50.0,50.0 });

	collisionMesh->AttachToComponent(PlaneMesh, FAttachmentTransformRules::KeepRelativeTransform);

	collisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectPawn::OnOverlapBegin);
	collisionMesh->OnComponentEndOverlap.AddDynamic(this, &AMyProjectPawn::OnOverlapEnd);

	cableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));

	cableComponent->AttachToComponent(PlaneMesh, FAttachmentTransformRules::KeepRelativeTransform);

	cableComponent->bAttachEnd = false;

}

void AMyProjectPawn::Tick(float DeltaSeconds)
{

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}


void AMyProjectPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &AMyProjectPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMyProjectPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectPawn::MoveRightInput);
}

void AMyProjectPawn::ThrustInput(float Val)
{
	auto newLocation = GetActorLocation();
	newLocation.Z += Val * 10;
	SetActorLocation(newLocation);
}

void AMyProjectPawn::MoveUpInput(float Val)
{
	auto newLocation = GetActorLocation();

	newLocation += (GetActorForwardVector() * Val) * 10;

	SetActorLocation(newLocation);
}

void AMyProjectPawn::MoveRightInput(float Val)
{
	auto newRotation = GetActorRotation();
	newRotation.Yaw += (Val * 0.5);
	SetActorRotation(newRotation);
}

void AMyProjectPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap!"));
}

void AMyProjectPawn::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NotOverlap!"));
}
