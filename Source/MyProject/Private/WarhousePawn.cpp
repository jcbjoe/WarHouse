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
#include "Components/DecalComponent.h"
#include "PhysicsProp.h"
#include "DestructibleProp.h"

const FName AWarhousePawn::MoveForwardBinding("MoveForward");
const FName AWarhousePawn::MoveRightBinding("MoveRight");
const FName AWarhousePawn::PickupBinding("PickupDrop");

const FName AWarhousePawn::ArmForwardBinding("ArmForward");
const FName AWarhousePawn::ArmRightBinding("ArmRight");

const FName AWarhousePawn::LeftTrigger("LeftTrigger");
const FName AWarhousePawn::RightTrigger("RightTrigger");

AWarhousePawn::AWarhousePawn()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> redMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerRed.PlayerRed"));
	static ConstructorHelpers::FObjectFinder<UMaterial> blueMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerBlue.PlayerBlue"));
	static ConstructorHelpers::FObjectFinder<UMaterial> yellowMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerYellow.PlayerYellow"));
	static ConstructorHelpers::FObjectFinder<UMaterial> whiteMat(TEXT("/Game/Assets/ConorAssets/Player/PlayerWhite.PlayerWhite"));

	red = redMat.Object;
	blue = blueMat.Object;
	yellow = yellowMat.Object;
	white = whiteMat.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Assets/ConorAssets/Player-Body.Player-Body"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BeamMesh(TEXT("/Game/Assets/ConorAssets/Player-GravBeam.Player-GravBeam"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	BeamMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
	BeamMeshComponent->SetStaticMesh(BeamMesh.Object);
	BeamMeshComponent->SetupAttachment(RootComponent);
	// Movement
	MoveSpeed = 1000.0f;
	DefaultMoveSpeed = 1000.0f;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	HeldLocation = CreateDefaultSubobject<USceneComponent>(FName("HoldLocation"));

	HeldLocation->AttachToComponent(ShipMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//HeldLocation->SetRelativeLocation({ 150,0,0, });
	HeldLocation->SetRelativeLocation({ 150,0,0, });

	ConstructorHelpers::FObjectFinder<UParticleSystem> emitter(TEXT("/Game/Assets/JoeAssets/Beam/Beam.Beam"));

	UParticleSystem* templateEmitter = emitter.Object;


	beamEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));
	beamEmitter->Template = templateEmitter;

	beamEmitter->SecondsBeforeInactive = 0;

	beamEmitter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	progressBar = CreateDefaultSubobject<UWidgetComponent>(FName("ProgressBar"));

	progressBar->SetupAttachment(RootComponent);

	progressBar->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> progressbarWidget(TEXT("/Game/UI/PackageCollectionBar"));

	progressBar->SetWidgetClass(progressbarWidget.Class);

	progressBar->SetDrawSize(FVector2D(200, 30));

	static ConstructorHelpers::FObjectFinder<UMaterial> decalMat(TEXT("/Game/Assets/JoeAssets/FloorMarker/FloorDecal.FloorDecal"));

	floorDecal = CreateDefaultSubobject<UDecalComponent>(L"Decal");

	floorDecal->SetDecalMaterial(decalMat.Object);

	floorDecal->SetVisibility(false);

	floorDecal->DecalSize = FVector(100, 40, 40);
	floorDecal->SetRelativeRotation(FRotator::MakeFromEuler({ 0, 90, 0 }));

	floorDecal->SetupAttachment(RootComponent);

	audioComp = CreateDefaultSubobject<UAudioComponent>(FName("Audio"));

	static ConstructorHelpers::FObjectFinder<USoundWave> engineSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Mechanical_Sounds/wav/Engine2Loop.Engine2Loop"));
	static ConstructorHelpers::FObjectFinder<USoundWave> dieSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Electric_Sounds/wav/electric_spark_burst__2_.electric_spark_burst__2_"));

	engineSoundBase = engineSound.Object;
	dieSoundBase = dieSound.Object;

	audioComp->SetSound(engineSoundBase);

	audioComp->SetVolumeMultiplier(audioStationaryVolume);

	static ConstructorHelpers::FObjectFinder<USoundWave> beamSound(TEXT("/Game/Sounds/Beam.Beam"));

	beamAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("BeamAudio"));

	beamAudioComp->SetSound(beamSound.Object);

	beamAudioComp->SetVolumeMultiplier(0.0f);

	static ConstructorHelpers::FObjectFinder<USoundWave> chargingSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Mechanical_Sounds/wav/engine_loop_6.engine_loop_6"));

	chargingComp = CreateDefaultSubobject<UAudioComponent>(FName("ChargingAudio"));

	chargingComp->SetSound(chargingSound.Object);

	chargingComp->SetVolumeMultiplier(0.0f);

}

void AWarhousePawn::BeginPlay()
{
	Super::BeginPlay();

	audioComp->Play();

	beamAudioComp->Play();

	chargingComp->Play();
}

void AWarhousePawn::SetColour(EPlayerColours colour)
{
	switch (colour)
	{
	case EPlayerColours::White:
		ShipMeshComponent->SetMaterial(0, white);
		BeamMeshComponent->SetMaterial(0, white);
		beamEmitter->SetColorParameter("Colour", FLinearColor::White);
		break;
	case EPlayerColours::Red:
		ShipMeshComponent->SetMaterial(0, red);
		BeamMeshComponent->SetMaterial(0, red);
		beamEmitter->SetColorParameter("Colour", FLinearColor::Red);
		break;
	case EPlayerColours::Blue:
		ShipMeshComponent->SetMaterial(0, blue);
		BeamMeshComponent->SetMaterial(0, blue);
		beamEmitter->SetColorParameter("Colour", FLinearColor::Blue);
		break;
	case EPlayerColours::Yellow:
		ShipMeshComponent->SetMaterial(0, yellow);
		BeamMeshComponent->SetMaterial(0, yellow);
		beamEmitter->SetColorParameter("Colour", FLinearColor::Yellow);
		break;
	}
}

void AWarhousePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);

	PlayerInputComponent->BindAxis(ArmForwardBinding);
	PlayerInputComponent->BindAxis(ArmRightBinding);

	PlayerInputComponent->BindAxis(LeftTrigger);
	PlayerInputComponent->BindAxis(RightTrigger);
}

void AWarhousePawn::Tick(float DeltaSeconds)
{
	if (!isDead) {
		//--- Grab controller input values
		const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
		const float RightValue = GetInputAxisValue(MoveRightBinding);

		const float ArmForwardValue = GetInputAxisValue(ArmForwardBinding);
		const float ArmRightValue = GetInputAxisValue(ArmRightBinding);

		const float LeftTriggerVal = GetInputAxisValue(LeftTrigger);
		const float RightTriggerVal = GetInputAxisValue(RightTrigger);

		//--- Calculate the yaw value from the right and forward of the left controller stick
		auto stickYaw = 360 - (FMath::RadiansToDegrees(FMath::Atan2(ArmRightValue, ArmForwardValue)) + 180);

		//--- Calculate the target X,Y coords for where the attached/held object will be located
		float targetX = (packageHoldDistance * FMath::Cos(stickYaw * UKismetMathLibrary::GetPI() / 180.f)) + GetActorLocation().X;
		float targetY = (packageHoldDistance * FMath::Sin(stickYaw * UKismetMathLibrary::GetPI() / 180.f)) + GetActorLocation().Y;

		const int beamHeightOffset = 137;

		//--- Create the vector of the target point and a copy. The copy is for the belt of the player that rotates
		FVector targetLocation = FVector(targetX, targetY, GetActorLocation().Z);
		FVector targetLocationCopy = targetLocation;

		targetLocation.Z += beamHeightOffset;

		//--- Adjust the height of the target based ont he triggers of the controller
		targetLocation.Z -= UKismetMathLibrary::MapRangeClamped(LeftTriggerVal, 0, 1, 0, maxUpDownVal);
		targetLocation.Z += UKismetMathLibrary::MapRangeClamped(RightTriggerVal, 0, 1, 0, maxUpDownVal);

		PhysicsHandle->SetTargetLocation(targetLocation);

		FVector beamSourceLocation = GetActorLocation();
		beamSourceLocation.Z += beamHeightOffset;
		beamEmitter->SetBeamSourcePoint(0, beamSourceLocation, 0);

		//--- If we have a package, set the beam location to it. If not set the beam location to the calculated target
		if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
			beamEmitter->SetBeamTargetPoint(0, targetLocation, 0);
		}
		else {
			beamEmitter->SetBeamTargetPoint(0, PhysicsHandle->GetGrabbedComponent()->GetComponentLocation(), 0);
		}

		//--- Set the rotation of the "BeamMeshComponent" this is the belt on the player which rotates
		FVector vec = (targetLocationCopy - GetActorLocation());
		vec.Normalize();
		BeamMeshComponent->SetWorldRotation(vec.Rotation());

		//--- Check for if the thumbsticks are currently being moved
		if (ArmForwardValue == 0 && ArmRightValue == 0)
		{
			//--- Thumbsticks idle (Not moving)
			//--- Silence beam sfx, hide beam emitter and floor decal (Crosshair)
			beamAudioComp->SetVolumeMultiplier(0.0f);
			beamEmitter->SetVisibility(false);
			floorDecal->SetVisibility(false);

			//--- Logic to drop the package if one is being held
			if (PhysicsHandle->GetGrabbedComponent() != nullptr)
			{
				DropHeldItem();
			}
		}
		else
		{
			//--- Thumbsticks moving
			//--- Show beam and set volume of the beam 
			beamAudioComp->SetVolumeMultiplier(audioBeamVolume);
			beamEmitter->SetVisibility(true);

			//--- Check if an item is held
			if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
				//--- Item not held
				//
				//--- Pickup package logic

				//--- Setup trace params
				FHitResult hit = FHitResult(ForceInit);
				FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);

				//--- Loop through every actor in the world, If it is not a package add it to the ignored actor list
				//--- The ignored actor list is a list of actors which will NOT trigger a hit on the trace
				for (TActorIterator<AActor> actor(GetWorld()); actor; ++actor)
				{
					if (!actor->IsA(APackageBase::StaticClass()) && !actor->IsA(APhysicsProp::StaticClass())) {
						TraceParams.AddIgnoredActor(*actor);
					}
				}

				//--- Create the line trace
				bool bIsHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), targetLocation, ECC_GameTraceChannel3, TraceParams);
				if (bIsHit)
				{
					//--- We are colliding with a actor

					UPrimitiveComponent* component = Cast<UPrimitiveComponent>(hit.GetActor()->GetRootComponent());

					//--- If the actor is a Package
					if (hit.Actor != nullptr && hit.Actor->IsA(APackageBase::StaticClass()))
					{
						PhysicsHandle->GrabComponentAtLocationWithRotation(component, "None", component->GetComponentLocation(), component->GetComponentRotation());
						APackageBase* package = Cast<APackageBase>(hit.GetActor());
						package->StartHolding(this);
					}

					//--- If the actor is a Physics prob (Object that can be picked up)
					if (hit.Actor != nullptr && hit.Actor->IsA(APhysicsProp::StaticClass()))
					{
						APhysicsProp* prop = Cast<APhysicsProp>(hit.GetActor());
						if (prop->GetCanPickUp())
						{
							PhysicsHandle->GrabComponentAtLocationWithRotation(component, "None", component->GetComponentLocation(), component->GetComponentRotation());
						}
					}
				}
			}
			else
			{
				//--- A item is being held - Calculate and show the floor decal
				FVector floorDecalPos = PhysicsHandle->GetGrabbedComponent()->GetComponentLocation();

				floorDecalPos.Z = 0;
				floorDecal->SetWorldLocation(floorDecalPos);

				floorDecal->SetVisibility(true);
			}
		}


		//--- Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

		//--- Calculate player movement
		const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

		//--- If non-zero size, move this actor
		if (Movement.SizeSquared() > 0.0f)
		{
			//--- Change the audio volume when moving
			audioComp->SetVolumeMultiplier(audioMovingVolume);

			//--- Calculate rotation of the drone
			const FRotator NewRotation = Movement.Rotation();
			FRotator rot = FMath::Lerp(GetActorRotation(), NewRotation, 0.05f);

			//--- Move the component and check if its colliding with something
			FHitResult Hit(1.f);
			RootComponent->MoveComponent(Movement, rot, true, &Hit);

			//--- Decrement the battery drain using the moving drain
			_batteryCharge -= (MovingBatteryDrain * DeltaSeconds);

			//--- Check if we are colliding and that the actor colliding with is not a package. Dont want to deflect from the package
			if (Hit.IsValidBlockingHit() && !Hit.Actor->GetClass()->IsChildOf(APackageBase::StaticClass()))
			{
				//--- Calculate the deflection
				const FVector Normal2D = Hit.Normal.GetSafeNormal2D() * 1.3;
				const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
				RootComponent->MoveComponent(Deflection, rot, true);
			}
		}
		else
		{
			//--- Set the volume back to the idle level and decrement the battery using the idle drain
			audioComp->SetVolumeMultiplier(audioStationaryVolume);
			_batteryCharge -= (NonMovingBatteryDrain * DeltaSeconds);
		}

		//--- Check if an object is being held
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
		{
			AActor* heldActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();

			if (heldActor->IsA(APackageBase::StaticClass()))
			{
				APackageBase* package = Cast<APackageBase>(heldActor);

				//--- Decrement the battery depending on how many people are holding the same package
				if (package->GetHeldBy().Num() > 1) {
					_batteryCharge -= (MultiHoldingBatteryDrain * DeltaSeconds);
				}
				else
				{
					_batteryCharge -= (SingleHoldingBatteryDrain * DeltaSeconds);
				}

				//--- Set movement speed based on weight
				float weight = package->GetPackageWeight();
				MoveSpeed = DefaultMoveSpeed - weight;

				//--- If the distance is 350 away stop holding the package
				float distance = FVector::Dist(GetActorLocation(), package->GetActorLocation());
				if (distance > 350) DropHeldItem();
			}

		}
		else {
			MoveSpeed = DefaultMoveSpeed;
		}

		//--- Set the battery bar
		UPackageProgressBar* playerBatteryBar = Cast<UPackageProgressBar>(progressBar->GetUserWidgetObject());
		playerBatteryBar->progressBarFillAmount = _batteryCharge / 100;

		//--- Calculate the rotation so the battery bar is pointing towards the camera
		const ACameraActor* cam = WarhouseHelpers::GetCameraManager(GetWorld())->GetCamera();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cam->GetActorLocation());
		rot.Yaw = 180;
		progressBar->SetWorldRotation(rot);

		//--- Set the battery bar location
		FVector newLoc = GetActorLocation();
		newLoc.Z = 10;
		progressBar->SetWorldLocation(newLoc);

		//--- Set the charging pad volume and the charge rate
		if (isOnChargingPad)
		{
			_batteryCharge += (chargingPadRate * DeltaSeconds);
			chargingComp->SetVolumeMultiplier(audioChargingVolume);
		}
		else
		{
			chargingComp->SetVolumeMultiplier(0.0f);
		}

		//--- Clamp the battery amount so it doesnt go above 100 or below 0
		_batteryCharge = FMath::Clamp(_batteryCharge, 0.0f, 100.0f);

		//--- Kill player check
		if (_batteryCharge <= 0)
		{
			//--- Player has ran out of battery
			isDead = true;
			respawnCounter = 0;

			if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
				DropHeldItem();
			}

			//--- Mute the engine sounds and play the death sound
			audioComp->SetVolumeMultiplier(0.0);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), dieSoundBase, GetActorLocation(), FRotator(0, 0, 0), deathSoundVolume);

			//--- Set the players position under the map and reset the beam
			auto pos = GetActorLocation();
			pos.Z = -600;

			SetActorLocation(pos);

			beamEmitter->SetBeamSourcePoint(0, pos, 0);

			beamEmitter->SetBeamTargetPoint(0, pos, 0);

			//Explosion?
		}
	}
	else
	{
		//-- If the player id dead increment the respawn counter by deltasecond (Time since last frame)
		//-- This allows us to count how long since the death.
		respawnCounter += DeltaSeconds;
		if (respawnCounter > respawnSeconds) {
			//--- We are at max time, lets respawn the player

			//--- Grab a random spawnpoint from the player manager
			const FVector spawnPoint = WarhouseHelpers::GetPlayerManager(GetWorld())->GetRandomSpawnpoint(true);

			SetActorLocation(spawnPoint);

			//--- Reset variables
			isDead = false;
			respawnCounter = 0;
			_batteryCharge = 100;
		}
	}

}

//--- This gets triggered by the charger actor on collider box enter
void AWarhousePawn::SetIsOnCharger(bool isOnCharger)
{
	isOnChargingPad = isOnCharger;
}

//--- Returns if the player is dead
bool AWarhousePawn::IsDead()
{
	return isDead;
}

//--- Properly drops the the item the player is holding
void AWarhousePawn::DropHeldItem()
{
	if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
		if (PhysicsHandle->GetGrabbedComponent()->GetOwner()->IsA(APackageBase::StaticClass()))
		{
			Cast<APackageBase>(PhysicsHandle->GetGrabbedComponent()->GetOwner())->EndHolding(this);
		}
		PhysicsHandle->ReleaseComponent();
	}
}
