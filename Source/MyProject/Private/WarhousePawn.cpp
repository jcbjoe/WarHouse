// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarhousePawn.h"

#include "PackageBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "EngineUtils.h"
#include "GameManager.h"
#include "MyPlayerController.h"
#include "PackageProgressBar.h"
#include "WarhouseHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "PhysicsProp.h"
#include "DestructibleProp.h"
#include "NiagaraFunctionLibrary.h"
#include "SettingsSave.h"
#include "Engine/LatentActionManager.h"

AWarhousePawn::AWarhousePawn()
{
	//--- Player materials setup
	static ConstructorHelpers::FObjectFinder<UMaterial> redMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/Red.Red"));
	static ConstructorHelpers::FObjectFinder<UMaterial> redGravMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/RedGrav.RedGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> blueMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/Blue.Blue"));
	static ConstructorHelpers::FObjectFinder<UMaterial> blueGravMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/BlueGrav.BlueGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> yellowMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/Yellow.Yellow"));
	static ConstructorHelpers::FObjectFinder<UMaterial> yellowGravMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/YellowGrav.YellowGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> whiteMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/Grey.Grey"));
	static ConstructorHelpers::FObjectFinder<UMaterial> whiteGravMat(TEXT("/Game/Assets/ConorAssets/Player/Colours/GreyGrav.GreyGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> glassesMat(TEXT("/Game/Assets/ConorAssets/Player/Glasses/GlassesBody.GlassesBody"));
	static ConstructorHelpers::FObjectFinder<UMaterial> glassesGravMat(TEXT("/Game/Assets/ConorAssets/Player/Glasses/GlassesGrav.GlassesGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> smileyMat(TEXT("/Game/Assets/ConorAssets/Player/Smiley/SmileyBody.SmileyBody"));
	static ConstructorHelpers::FObjectFinder<UMaterial> smileyGravMat(TEXT("/Game/Assets/ConorAssets/Player/Smiley/SmileyGrav.SmileyGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> tuxMat(TEXT("/Game/Assets/ConorAssets/Player/Tux/TuxBody.TuxBody"));
	static ConstructorHelpers::FObjectFinder<UMaterial> tuxGravMat(TEXT("/Game/Assets/ConorAssets/Player/Tux/TuxGrav.TuxGrav"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ironManMat(TEXT("/Game/Assets/ConorAssets/Player/IronMan/IronMan.IronMan"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ironManGravMat(TEXT("/Game/Assets/ConorAssets/Player/IronMan/IronManGrav.IronManGrav"));

	red = redMat.Object;
	redGrav = redGravMat.Object;
	blue = blueMat.Object;
	blueGrav = blueGravMat.Object;
	yellow = yellowMat.Object;
	yellowGrav = yellowGravMat.Object;
	white = whiteMat.Object;
	whiteGrav = whiteGravMat.Object;

	glasses = glassesMat.Object;
	glassesGrav = glassesGravMat.Object;
	smiley = smileyMat.Object;
	smileyGrav = smileyGravMat.Object;
	tux = tuxMat.Object;
	tuxGrav = tuxGravMat.Object;
	ironman = ironManMat.Object;
	ironmanGrav = ironManGravMat.Object;

	//--- Player model setup
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Assets/ConorAssets/Player/player_body_low.player_body_low"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);

	//--- Player belt/beam setup
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BeamMesh(TEXT("/Game/Assets/ConorAssets/Player/player_gravbeam_low.player_gravbeam_low"));
	BeamMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
	BeamMeshComponent->SetStaticMesh(BeamMesh.Object);
	BeamMeshComponent->SetupAttachment(RootComponent);

	//--- Physics handle setup
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	HeldLocation = CreateDefaultSubobject<USceneComponent>(FName("HoldLocation"));
	HeldLocation->SetupAttachment(ShipMeshComponent);
	HeldLocation->SetRelativeLocation({ 150,0,0, });

	//--- Beam emitter setup
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> nEmitter(TEXT("/Game/Assets/JoeAssets/Beam/RobotBeam_System.RobotBeam_System"));

	beamEmitter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Beam"));
	beamEmitter->SetAsset(nEmitter.Object);
	beamEmitter->SetupAttachment(RootComponent);

	//--- Sparks emitter setup
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> sparksEmitterSystem(TEXT("/Game/Assets/JoeAssets/Sparks/Sparks_System.Sparks_System"));

	sparksEmitter = sparksEmitterSystem.Object;

	//--- Battery bar setup
	progressBar = CreateDefaultSubobject<UWidgetComponent>(FName("ProgressBar"));
	progressBar->SetupAttachment(RootComponent);
	progressBar->SetWidgetSpace(EWidgetSpace::World);
	static ConstructorHelpers::FClassFinder<UUserWidget> progressbarWidget(TEXT("/Game/UI/PackageCollectionBar"));
	progressBar->SetWidgetClass(progressbarWidget.Class);
	progressBar->SetDrawSize(FVector2D(100, 30));

	//--- Floor crosshair/decal setup
	static ConstructorHelpers::FObjectFinder<UMaterial> decalMat(TEXT("/Game/Assets/JoeAssets/FloorMarker/FloorDecal.FloorDecal"));
	floorDecal = CreateDefaultSubobject<UDecalComponent>(L"Decal");
	floorDecal->SetDecalMaterial(decalMat.Object);
	floorDecal->SetVisibility(false);
	floorDecal->DecalSize = FVector(100, 40, 40);
	floorDecal->SetRelativeRotation(FRotator::MakeFromEuler({ 0, 90, 0 }));
	floorDecal->SetupAttachment(RootComponent);

	//--- Audio setup
	static ConstructorHelpers::FObjectFinder<USoundWave> engineSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Mechanical_Sounds/wav/Engine2Loop.Engine2Loop"));
	static ConstructorHelpers::FObjectFinder<USoundWave> dieSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Electric_Sounds/wav/electric_spark_burst__2_.electric_spark_burst__2_"));
	static ConstructorHelpers::FObjectFinder<USoundWave> chargingSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Mechanical_Sounds/wav/engine_loop_6.engine_loop_6"));
	static ConstructorHelpers::FObjectFinder<USoundWave> beamSound(TEXT("/Game/Sounds/Beam.Beam"));

	audioComp = CreateDefaultSubobject<UAudioComponent>(FName("Audio"));
	beamAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("BeamAudio"));
	chargingComp = CreateDefaultSubobject<UAudioComponent>(FName("ChargingAudio"));

	dieSoundBase = dieSound.Object;

	audioComp->SetSound(engineSound.Object);
	beamAudioComp->SetSound(beamSound.Object);
	chargingComp->SetSound(chargingSound.Object);

	beamAudioComp->SetVolumeMultiplier(0.0f);
	chargingComp->SetVolumeMultiplier(0.0f);

	//--- Create beam locations
	beamSource = CreateDefaultSubobject<USceneComponent>(FName("SourcePos"));
	beamSource->SetupAttachment(RootComponent);

	beamTarget = CreateDefaultSubobject<USceneComponent>(FName("TargetPos"));
	beamTarget->SetupAttachment(RootComponent);
}

void AWarhousePawn::BeginPlay()
{
	Super::BeginPlay();
	audioComp->Play();
	beamAudioComp->Play();
	chargingComp->Play();
	//add collision function for when a hit is detected
	ShipMeshComponent->OnComponentHit.AddDynamic(this, &AWarhousePawn::OnHit);
	//load in user settings
	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		volumeMultiplier = LoadedGame->SFXVolume;
		canVibrate = LoadedGame->VibrationEnabled;
	}

	audioComp->SetVolumeMultiplier(audioStationaryVolume * volumeMultiplier);
}

void AWarhousePawn::SetColour(EPlayerColours colour)
{
	switch (colour)
	{
	case EPlayerColours::White:
		ShipMeshComponent->SetMaterial(0, white);
		BeamMeshComponent->SetMaterial(0, whiteGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("dceafe"));
		break;
	case EPlayerColours::Red:
		ShipMeshComponent->SetMaterial(0, red);
		BeamMeshComponent->SetMaterial(0, redGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("e41d20"));
		break;
	case EPlayerColours::Blue:
		ShipMeshComponent->SetMaterial(0, blue);
		BeamMeshComponent->SetMaterial(0, blueGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("3e59a6"));
		break;
	case EPlayerColours::Yellow:
		ShipMeshComponent->SetMaterial(0, yellow);
		BeamMeshComponent->SetMaterial(0, yellowGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("dfae00"));
		break;
	case EPlayerColours::glasses:
		ShipMeshComponent->SetMaterial(0, glasses);
		BeamMeshComponent->SetMaterial(0, glassesGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("e57b07"));
		break;
	case EPlayerColours::smiley:
		ShipMeshComponent->SetMaterial(0, smiley);
		BeamMeshComponent->SetMaterial(0, smileyGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("3d559f"));
		break;
	case EPlayerColours::tux:
		ShipMeshComponent->SetMaterial(0, tux);
		BeamMeshComponent->SetMaterial(0, tuxGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("323234"));
		break;
	case EPlayerColours::ironman:
		ShipMeshComponent->SetMaterial(0, ironman);
		BeamMeshComponent->SetMaterial(0, ironmanGrav);
		beamEmitter->SetColorParameter("Colour", FColor::FromHex("883436"));
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
	Super::Tick(DeltaSeconds);

	if (!isDead) {
		//--- Grab controller input values
		const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
		const float RightValue = GetInputAxisValue(MoveRightBinding);

		const float ArmForwardValue = GetInputAxisValue(ArmForwardBinding);
		const float ArmRightValue = GetInputAxisValue(ArmRightBinding);

		const float LeftTriggerVal = GetInputAxisValue(LeftTrigger);
		const float RightTriggerVal = GetInputAxisValue(RightTrigger);

		//--- Calculate additional height
		float additionalHeight = 0;
		additionalHeight -= UKismetMathLibrary::MapRangeClamped(LeftTriggerVal, 0, 1, 0, maxUpDownVal);
		additionalHeight += UKismetMathLibrary::MapRangeClamped(RightTriggerVal, 0, 1, 0, maxUpDownVal);

		//--- Calculate the beam target and source locations
		const FVector ArmDirection = FVector(-ArmForwardValue, ArmRightValue, 0.f).GetClampedToMaxSize(1.0f);
		FVector sourceLoc = (ArmDirection * 40) + GetActorLocation();
		FVector targetLoc = (ArmDirection * 200) + GetActorLocation();
		sourceLoc.Z = GetActorLocation().Z + beamHeightOffset;
		targetLoc.Z = GetActorLocation().Z + beamHeightOffset + additionalHeight;

		//--- Set the target and source
		beamSource->SetWorldLocation(sourceLoc);
		beamTarget->SetWorldLocation(targetLoc);

		//--- Set target for the Physics handle
		PhysicsHandle->SetTargetLocation(beamTarget->GetComponentLocation());

		//--- Setup Emitter locations
		beamEmitter->SetVectorParameter(FName("Start"), beamSource->GetComponentLocation());

		//--- If we have a package, set the beam location to it. If not set the beam location to the calculated target
		if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
			beamEmitter->SetVectorParameter(FName("End"), beamTarget->GetComponentLocation());
		}
		else {

			beamEmitter->SetVectorParameter(FName("End"), PhysicsHandle->GetGrabbedComponent()->GetComponentLocation());
		}

		//--- Check for if the thumbsticks are currently being moved
		if (ArmForwardValue > -0.25 && ArmForwardValue < 0.25 && ArmRightValue > -0.25 && ArmRightValue < 0.25)
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

			BeamMeshComponent->SetWorldRotation(ArmDirection.Rotation());
			beamAudioComp->SetVolumeMultiplier(audioBeamVolume * volumeMultiplier);
			beamEmitter->SetVisibility(true);
			//play controller rumble
			if (canVibrate) {

				const float dir = FVector(-ArmForwardValue, ArmRightValue, 0.f).GetClampedToMaxSize(1.0f).Size();
				const float dirDivide = dir / 10;

				AMyPlayerController* pc = Cast<AMyPlayerController>(GetController());
				pc->PlayDynamicForceFeedback(dirDivide, RumbleDuration, true, false, true, false, EDynamicForceFeedbackAction::Start); // change first 2 floats for vibration intenisty and duration, 4 bools are diff motors
			}
			//--- Check if an item is held
			float distance = FVector::Dist(sourceLoc, targetLoc);
			if (PhysicsHandle->GetGrabbedComponent() == nullptr) {
				//--- Item not held

				floorDecal->SetVisibility(false);

				//--- Pickup package logic
				if (distance > 150) {
					//--- Setup trace params
					FHitResult hit;
					//FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);

					////--- Loop through every actor in the world, If it is not a package add it to the ignored actor list
					////--- The ignored actor list is a list of actors which will NOT trigger a hit on the trace
					//for (TActorIterator<AActor> actor(GetWorld()); actor; ++actor)
					//{
					//	if (!actor->IsA(APackageBase::StaticClass()) && !actor->IsA(APhysicsProp::StaticClass())) {
					//		TraceParams.AddIgnoredActor(*actor);
					//	}
					//}

					//--- Create the line trace
					bool bIsHit = GetWorld()->LineTraceSingleByChannel(hit, beamSource->GetComponentLocation(), beamTarget->GetComponentLocation(), ECC_GameTraceChannel1);
					if (bIsHit && hit.Actor != nullptr)
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
			}
		}


		//--- Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

		//--- Calculate player movement
		const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

		//--- If non-zero size, move this actor
		if (Movement.SizeSquared() > 0.0f)
		{
			//vibrate on movement
			if (canVibrate) {
				auto pc = Cast<AMyPlayerController>(GetController());
				pc->PlayDynamicForceFeedback(MovementRumble, RumbleDuration, true, true, true, true, EDynamicForceFeedbackAction::Start);
			}
			//--- Change the audio volume when moving
			audioComp->SetVolumeMultiplier(audioMovingVolume * volumeMultiplier);

			//--- Calculate rotation of the drone
			const FRotator NewRotation = Movement.Rotation();
			FRotator rot = FMath::Lerp(GetActorRotation(), NewRotation, 0.05f);

			//--- Move the component and check if its colliding with something
			FHitResult Hit(1.f);
			RootComponent->MoveComponent(Movement, rot, true, &Hit);

			//--- Decrement the battery drain using the moving drain
			_batteryCharge -= (MovingBatteryDrain * DeltaSeconds);

			//--- Check if we are colliding and that the actor colliding with is not a package. Dont want to deflect from the package
			if (Hit.IsValidBlockingHit() && Hit.Actor != nullptr && !Hit.Actor->IsA(APackageBase::StaticClass()))
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
			audioComp->SetVolumeMultiplier(audioStationaryVolume * volumeMultiplier);
			_batteryCharge -= (NonMovingBatteryDrain * DeltaSeconds);
		}

		//--- Check if an object is being held
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
		{
			AActor* heldActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();

			//--- A item is being held - Calculate and show the floor decal
			FVector floorDecalPos = PhysicsHandle->GetGrabbedComponent()->GetComponentLocation();

			floorDecalPos.Z = 0;
			floorDecal->SetWorldLocation(floorDecalPos);

			floorDecal->SetVisibility(true);

			if (Cast<AMyPlayerController>(GetController())->IsInputKeyDown(EKeys::Gamepad_LeftShoulder))
			{
				DropHeldItem();
			}

			//--- If the distance is 350 away stop holding the package
			float distance = FVector::Dist(beamTarget->GetComponentLocation(), heldActor->GetActorLocation());

			if (distance > 300) {
				DropHeldItem();
			}

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
				MoveSpeed = DefaultMoveSpeed - (weight * 10);
			}
		}
		else {
			MoveSpeed = DefaultMoveSpeed;
		}

		//--- Set the battery bar
		UPackageProgressBar* playerBatteryBar = Cast<UPackageProgressBar>(progressBar->GetUserWidgetObject());
		playerBatteryBar->progressBarFillAmount = _batteryCharge / 100;

		//--- Calculate the rotation so the battery bar is pointing towards the camera
		const ACameraActor* cam = WarhouseHelpers::GetCameraManager(GetWorld())->GetMainCamera();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(progressBar->GetComponentLocation(), cam->GetActorLocation());
		progressBar->SetWorldRotation(rot);

		//--- Set the battery bar location
		progressBar->SetRelativeLocation({ 0,0,275 });

		//--- Set the charging pad volume and the charge rate
		if (isOnChargingPad)
		{
			if (canVibrate) {
				auto pc = Cast<AMyPlayerController>(GetController());
				pc->PlayDynamicForceFeedback(ChargingPadRumble, RumbleDuration, true, true, true, true, EDynamicForceFeedbackAction::Start);
			}
			_batteryCharge += (chargingPadRate * DeltaSeconds);
			chargingComp->SetVolumeMultiplier(audioChargingVolume * volumeMultiplier);
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
			KillPlayer();
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
			lastHeldPackage = PhysicsHandle->GetGrabbedComponent()->GetOwner();
			Cast<APackageBase>(PhysicsHandle->GetGrabbedComponent()->GetOwner())->EndHolding(this);
		}
		PhysicsHandle->ReleaseComponent();
	}
}

void AWarhousePawn::KillPlayer()
{
	if (canVibrate) {
		auto pc = Cast<AMyPlayerController>(GetController());
		pc->PlayDynamicForceFeedback(DeathRumble, RumbleDuration, true, true, true, true, EDynamicForceFeedbackAction::Start);
	}
	//--- Player has ran out of battery
	isDead = true;
	respawnCounter = 0;

	if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
		DropHeldItem();
	}

	//--- Mute the engine sounds and play the death sound
	audioComp->SetVolumeMultiplier(0.0);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), dieSoundBase, GetActorLocation(), FRotator(0, 0, 0), deathSoundVolume);

	beamAudioComp->SetVolumeMultiplier(0.0);

	//--- Trigger sparks
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), sparksEmitter, GetActorLocation());

	//--- Set the players position under the map and reset the beam
	auto pos = GetActorLocation();
	pos.Z = -600;

	SetActorLocation(pos);

	beamEmitter->SetVectorParameter(FName("Start"), pos);
	beamEmitter->SetVectorParameter(FName("End"), pos);
}

void AWarhousePawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (OtherActor != nullptr && OtherActor->IsA(APackageBase::StaticClass()))
	//{
	//	if ((PhysicsHandle->GetGrabbedComponent() != nullptr && PhysicsHandle->GetGrabbedComponent()->GetOwner() == OtherActor)
	//		|| OtherActor == lastHeldPackage)
	//	{
	//		DropHeldItem();
	//		Cast<UStaticMeshComponent>(OtherActor->GetRootComponent())->SetSimulatePhysics(false);
	//		Cast<UStaticMeshComponent>(OtherActor->GetRootComponent())->SetSimulatePhysics(true);

	//		lastHeldPackage = nullptr;
	//	}
	//}
}
