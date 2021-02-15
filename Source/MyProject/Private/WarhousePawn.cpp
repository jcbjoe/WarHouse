// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarhousePawn.h"

#include <string>

#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundBase.h"

const FName AWarhousePawn::MoveForwardBinding("MoveForward");
const FName AWarhousePawn::MoveRightBinding("MoveRight");
const FName AWarhousePawn::PickupBinding("PickupDrop");


AWarhousePawn::AWarhousePawn()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Movement
	MoveSpeed = 1000.0f;

	collisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	collisionMesh->SetRelativeLocation({ 16.0,0.0,7.0 });
	collisionMesh->SetBoxExtent({ 88.0,50.0,35.0 });

	collisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	collisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AWarhousePawn::OnOverlapBegin);

	collisionMesh->OnComponentEndOverlap.AddDynamic(this, &AWarhousePawn::OnOverlapEnd);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	HeldLocation = CreateDefaultSubobject<USceneComponent>(FName("HoldLocation"));

	HeldLocation->AttachToComponent(ShipMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HeldLocation->SetRelativeLocation({ 150,0,0, });

}


void AWarhousePawn::BeginPlay()
{
	Super::BeginPlay();

	auto location = GetActorLocation();

	location.Z = 195;

	SetActorLocation(location);
}

void AWarhousePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAction(PickupBinding, IE_Pressed, this, &AWarhousePawn::OnPickupPressed);
}

void AWarhousePawn::Tick(float DeltaSeconds)
{
	PhysicsHandle->SetTargetLocation(HeldLocation->GetComponentLocation());

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();

		auto rot = FMath::Lerp(GetActorRotation(), NewRotation, 0.05f);
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, rot, true, &Hit);

		if (Hit.IsValidBlockingHit() && (Hit.Actor == nullptr || !Hit.Actor->GetClass()->IsChildOf(AWarehousePackage::StaticClass())))
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D() * 1.3;
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, rot, true);
		}
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		float distance = FVector::Dist(GetActorLocation(), PhysicsHandle->GetGrabbedComponent()->GetComponentLocation());
		if (distance > 350)
		{
			// Package is to far away, drop it!
			PhysicsHandle->ReleaseComponent();
		}
	}

}

void AWarhousePawn::OnPickupPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Package pickup pressed"));

	if (isCollidingPackage && PhysicsHandle->GetGrabbedComponent() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Picking up"));
		isCollidingPackage = false;

		PhysicsHandle->GrabComponentAtLocation(objCollidingWith, "None", objCollidingWith->GetComponentLocation());

	}
	else if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
		PhysicsHandle->ReleaseComponent();
	}

}

void AWarhousePawn::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	auto otherActorName = OtherActor->GetName();


	if (OtherActor->IsA(AWarehousePackage::StaticClass())) {
		UE_LOG(LogTemp, Warning, TEXT("ITS A PACKAGE"));
		isCollidingPackage = true;
		objCollidingWith = OtherComp;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ITS NOT PACKAGE :("));
	}

}

void AWarhousePawn::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (isCollidingPackage) {
		if (OtherComp == objCollidingWith) {
			objCollidingWith = nullptr;
			isCollidingPackage = false;
		}
	}
}