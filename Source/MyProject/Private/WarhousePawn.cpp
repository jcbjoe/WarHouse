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
#include "GameManager.h"
#include "PackageProgressBar.h"
#include "PackageBase.h"
#include "WarhouseHelpers.h"

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

	progressBar = CreateDefaultSubobject<UWidgetComponent>(FName("ProgressBar"));

	progressBar->SetupAttachment(RootComponent);

	progressBar->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> progressbarWidget(TEXT("/Game/UI/PackageCollectionBar"));

	progressBar->SetWidgetClass(progressbarWidget.Class);

	progressBar->SetDrawSize(FVector2D(200, 30));

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

	PlayerInputComponent->BindAxis(ArmForwardBinding);
	PlayerInputComponent->BindAxis(ArmRightBinding);
}

void AWarhousePawn::Tick(float DeltaSeconds)
{
	if (!isDead) {
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
		if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
			beamEmitter->SetBeamTargetPoint(0, trans, 0);
		}
		else
		{
			beamEmitter->SetBeamTargetPoint(0, PhysicsHandle->GetGrabbedComponent()->GetComponentLocation(), 0);
		}

		if (ArmForwardValue == 0 && ArmRightValue == 0)
		{
			beamEmitter->SetVisibility(false);
			if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
				// Player has let go of package
				reinterpret_cast<APackageBase*>(PhysicsHandle->GetGrabbedComponent()->GetOwner())->EndHolding(this);
				
				PhysicsHandle->ReleaseComponent();

			}
		}
		else
		{
			beamEmitter->SetVisibility(true);

			if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
				FHitResult hit = FHitResult(ForceInit);
				FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);

				for (TActorIterator<AActor> actor(GetWorld()); actor; ++actor)
				{
					if (!actor->IsA(APackageBase::StaticClass())) {
						TraceParams.AddIgnoredActor(*actor);
					}
				}

				bool bIsHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), trans, ECC_GameTraceChannel3, TraceParams);
				if (bIsHit)
				{
					if (hit.Actor != nullptr && hit.Actor->IsA(APackageBase::StaticClass())) {
						UPrimitiveComponent* component = reinterpret_cast<UPrimitiveComponent*>(hit.GetActor()->GetRootComponent());
						PhysicsHandle->GrabComponentAtLocationWithRotation(component, "None", component->GetComponentLocation(), component->GetComponentRotation());
						auto package = reinterpret_cast<APackageBase*>(hit.GetActor());
						package->StartHolding(this);
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

			if (Hit.IsValidBlockingHit() && (Hit.Actor == nullptr || !Hit.Actor->GetClass()->IsChildOf(APackageBase::StaticClass())))
			{
				const FVector Normal2D = Hit.Normal.GetSafeNormal2D() * 1.3;
				const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
				RootComponent->MoveComponent(Deflection, rot, true);
			}

			_batteryCharge -= (MovingBatteryDrain * DeltaSeconds);
		}
		else
		{

			_batteryCharge -= (NonMovingBatteryDrain * DeltaSeconds);
		}

		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
		{
			auto package = reinterpret_cast<APackageBase*>(PhysicsHandle->GetGrabbedComponent()->GetOwner());
			if (package->GetHeldBy().Num() > 1) {
				_batteryCharge -= (MultiHoldingBatteryDrain * DeltaSeconds);
			} else
			{
				_batteryCharge -= (SingleHoldingBatteryDrain * DeltaSeconds);
			}
			float distance = FVector::Dist(GetActorLocation(), package->GetActorLocation());
			if (distance > 350)
			{
				//Package is to far away, drop it!
				package->EndHolding(this);

				PhysicsHandle->ReleaseComponent();
			}
		}

		reinterpret_cast<UPackageProgressBar*>(progressBar->GetUserWidgetObject())->progressBarFillAmount = _batteryCharge / 100;

		const ACameraActor* cam = WarhouseHelpers::GetCameraManager(GetWorld())->GetCamera();

		auto rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cam->GetActorLocation());
		rot.Yaw = 180;
		progressBar->SetWorldRotation(rot);

		auto newLoc = GetActorLocation();
		newLoc.Z += 75;
		progressBar->SetWorldLocation(newLoc);

		if(isOnChargingPad)
		{
			_batteryCharge += (chargingPadRate * DeltaSeconds);
		}

		_batteryCharge = FMath::Clamp(_batteryCharge, 0.0f, 100.0f);
		
		if (_batteryCharge <= 0)
		{
			isDead = true;
			respawnCounter = 0;

			auto pos = GetActorLocation();
			pos.Z = -100;

			SetActorLocation(pos);

			beamEmitter->SetBeamSourcePoint(0, pos, 0);

			beamEmitter->SetBeamTargetPoint(0, pos, 0);

			if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
				PhysicsHandle->ReleaseComponent();
			}


			//Explosion?

			//Death Logic (position)
		}
	}
	else
	{
		respawnCounter += DeltaSeconds;
		if (respawnCounter > respawnSeconds) {

			const FVector spawnPoint = WarhouseHelpers::GetPlayerManager(GetWorld())->GetRandomSpawnpoint();

			SetActorLocation(spawnPoint);

			isDead = false;
			respawnCounter = 0;
			_batteryCharge = 100;
		}
	}

}

void AWarhousePawn::SetIsOnCharger(bool isOnCharger)
{
	isOnChargingPad = isOnCharger;
}

bool AWarhousePawn::IsDead()
{
	return isDead;
}