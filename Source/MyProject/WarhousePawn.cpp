// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarhousePawn.h"
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
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

}

void AWarhousePawn::OnPickupPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Package pickup pressed"));

	if (isCollidingPackage && !hasPickedUpPackage) {
		UE_LOG(LogTemp, Warning, TEXT("Picking up"));
		hasPickedUpPackage = true;
		isCollidingPackage = false;
		pickedUpPackage = packageCollidingWith;
		packageCollidingWith = nullptr;

		auto test = this->GetAttachParentSocketName();
		pickedUpPackage->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), test);

		auto loc = pickedUpPackage->GetActorLocation();
		loc.X = 90;
		loc.Y = 0;
		loc.Z = 10;
		pickedUpPackage->SetActorRelativeLocation(loc);
	}
	else if (hasPickedUpPackage) {
		pickedUpPackage->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		pickedUpPackage->cubeMeshComponent->SetSimulatePhysics(true);
		hasPickedUpPackage = false;
		pickedUpPackage = nullptr;
	}
	
}

void AWarhousePawn::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	auto otherActorName = OtherActor->GetName();


	if (OtherActor->IsA(AWarehousePackage::StaticClass())) {
		UE_LOG(LogTemp, Warning, TEXT("ITS A PACKAGE"));
		isCollidingPackage = true;
		packageCollidingWith = Cast<AWarehousePackage>(OtherActor);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ITS NOT PACKAGE :("));
	}

}

void AWarhousePawn::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (isCollidingPackage) {
		if (OtherActor == packageCollidingWith) {
			packageCollidingWith = nullptr;
			isCollidingPackage = false;
		}
	}
}