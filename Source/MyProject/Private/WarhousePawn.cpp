// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarhousePawn.h"

#include <string>

#include "EditorCategoryUtils.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathVectorConstants.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "EngineUtils.h"

const FName AWarhousePawn::MoveForwardBinding("MoveForward");
const FName AWarhousePawn::MoveRightBinding("MoveRight");
const FName AWarhousePawn::PickupBinding("PickupDrop");

const FName AWarhousePawn::ArmForwardBinding("ArmForward");
const FName AWarhousePawn::ArmRightBinding("ArmRight");

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

	//HeldLocation->SetRelativeLocation({ 150,0,0, });
	HeldLocation->SetRelativeLocation({ 150,0,0, });

	ConstructorHelpers::FObjectFinder<UParticleSystem> emitter(TEXT("/Game/Assets/JoeAssets/Beam/Beam.Beam"));

	UParticleSystem* templateEmitter = emitter.Object;


	beamEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));
	beamEmitter->Template = templateEmitter;

	beamEmitter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

	PlayerInputComponent->BindAxis(ArmForwardBinding);
	PlayerInputComponent->BindAxis(ArmRightBinding);
}

void AWarhousePawn::Tick(float DeltaSeconds)
{

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const float ArmForwardValue = GetInputAxisValue(ArmForwardBinding);
	const float ArmRightValue = GetInputAxisValue(ArmRightBinding);

	auto yaw = 360 - (FMath::RadiansToDegrees(FMath::Atan2(ArmRightValue, ArmForwardValue)) + 180);

	float angle = yaw;

	float x = (150 * FMath::Cos(angle * UKismetMathLibrary::GetPI() / 180.f)) + GetActorLocation().X;
	float y = (150 * FMath::Sin(angle * UKismetMathLibrary::GetPI() / 180.f)) + GetActorLocation().Y;

	auto trans = FVector(x, y, GetActorLocation().Z);

	PhysicsHandle->SetTargetLocation(trans);

	auto target = GetActorLocation();
	target.Y += 10;
	beamEmitter->SetBeamSourcePoint(0, target, 0);
	beamEmitter->SetBeamTargetPoint(0, trans, 0);

	if(ArmForwardValue == 0 && ArmRightValue == 0)
	{
		beamEmitter->SetVisibility(false);
		if (isCollidingPackage && PhysicsHandle->GetGrabbedComponent() != nullptr) {
			PhysicsHandle->ReleaseComponent();
		}
	} else
	{
		beamEmitter->SetVisibility(true);

		if (isCollidingPackage && PhysicsHandle->GetGrabbedComponent() == nullptr) {
			FHitResult hit = FHitResult(ForceInit);
			FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);
			TraceParams.TraceTag = "Trace";

			for (TActorIterator<AActor> actor(GetWorld()); actor; ++actor)
			{
				if (!actor->IsA(AWarehousePackage::StaticClass())) {
					TraceParams.AddIgnoredActor(*actor);
				}
			}
			
			GetWorld()->DebugDrawTraceTag = "Trace";
			bool bIsHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), trans, ECC_GameTraceChannel3, TraceParams);
			if(bIsHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Raytrace hit!"));
				if (hit.Actor->IsA(AWarehousePackage::StaticClass())) {
					UE_LOG(LogTemp, Warning, TEXT("Raytrace hit - package!"));
					UPrimitiveComponent* component = reinterpret_cast<UPrimitiveComponent*>(hit.GetActor()->GetRootComponent());
					PhysicsHandle->GrabComponentAtLocation(component, "None", component->GetComponentLocation());
				}
			}
		}
	}


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