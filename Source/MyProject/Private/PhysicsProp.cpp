// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProp.h"
#include "DrawDebugHelpers.h"
#include "WarhousePawn.h"
#include "DestructibleComponent.h"
#include "DestructibleProp.h"
#include "SettingsSave.h"
// Sets default values
APhysicsProp::APhysicsProp()
{
	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//set up mesh
	PropMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	PropMeshComponent->SetNotifyRigidBodyCollision(true);
	PropMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	//set health
	PropHealth = 100.0f;
	//simulate physics
	PropMeshComponent->SetSimulatePhysics(true);
	RootComponent = PropMeshComponent;
	//set up particles
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleEmitter"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetVisibility(false);
	//set radial impact data
	ImpactRadius = 500.0f;
	RadialImpactForce = 2000.0f;
	//set up audio
	static ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("/Game/Extras/Audio/Explosion02.Explosion02"));
	soundBase = sound.Object;

}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();

	PropMeshComponent->SetMassOverrideInKg(NAME_None, 20);
	
	//stop particles playing
	ParticleSystemComponent->DeactivateSystem();
	//add collision function for when a hit is detected
	PropMeshComponent->OnComponentHit.AddDynamic(this, &APhysicsProp::OnHit);
	//check properties to see what behaviour should be allowed
	if (IsFragile)
	{
		PropHealth = 1.0f;
	}

	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		volumeMultiplier = LoadedGame->SFXVolume;
	}
}

// Called every frame
void APhysicsProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APhysicsProp::GetCanPickUp()
{
	return CanPickUp;
}

bool APhysicsProp::GetDestructible()
{
	return IsDestructible;
}

bool APhysicsProp::GetUseParticleEmitter()
{
	return UseParticleEmitter;
}

bool APhysicsProp::GetIsFragile()
{
	return IsFragile;
}

bool APhysicsProp::GetCanExplode()
{
	return CanExplode;
}

void APhysicsProp::ActivateParticles()
{
	//turn on visibilty and activate the system
	ParticleSystemComponent->SetVisibility(true);
	ParticleSystemComponent->ActivateSystem();
	//set timer to auto deactivate particle system
	GetWorld()->GetTimerManager().SetTimer(ParticlesTimerHandle, this, &APhysicsProp::DeactivateParticles, ParticleLife, false);
}

void APhysicsProp::DeactivateParticles()
{
	ParticleSystemComponent->DeactivateSystem();
	ParticleSystemComponent->SetVisibility(false);
}

void APhysicsProp::DestroyProp()
{
	this->Destroy();
}

void APhysicsProp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//get the velocity of this prop
		auto velocity = this->GetVelocity().Size();
		//if velocity is fast enough to register movement, deduct health from it
		if (velocity > 105 && !isPropDead && canRegisterHit)
		{
			PropHealth -= 5.0f;
		}
		//if this prop is using a particle system to be activated on death, activate it
		if (GetUseParticleEmitter() && PropHealth < 0.0f)
		{
			PropHealth = 0.0f;
			isPropDead = true;
			ActivateParticles();
		}
		//or if it can explode on death, call Explode()
		if (GetCanExplode() && PropHealth < 0.0f)
		{
			PropHealth = 0.0f;
			isPropDead = true;
			if (ParticleSystemComponent)
				ActivateParticles();
			Explode();
		}
		//reset timer to allow prop to be hit again, this will prevent multiple hits in a frame
		GetWorld()->GetTimerManager().SetTimer(AllowHitTimer, this, &APhysicsProp::AllowHit, 0.5f, false);
	}
}

void APhysicsProp::Explode()
{
	//play sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundBase, this->GetActorLocation(), 1.0f);
	//activate particles
	ActivateParticles();
	//get this props location
	FVector Location = this->GetActorLocation();
	//create a sphere collider to be used in the collision check
	FCollisionShape SphereCol = FCollisionShape::MakeSphere(ImpactRadius);
	//sweep for multiple actors at this location using the sphere collider and store hit actors into a TArray
	bool SweepHit = GetWorld()->SweepMultiByChannel(HitActors, Location, Location, FQuat::Identity, ECC_WorldStatic, SphereCol);
	if (SweepHit)
	{
		//loop through the array of hit actors
		for (auto& hit : HitActors)
		{
			//check if its a player and kill them
			if (hit.GetActor()->IsA(AWarhousePawn::StaticClass()))
			{
				AWarhousePawn* player = Cast<AWarhousePawn>(hit.GetActor());
				player->KillPlayer();
			}
			//get the mesh and apply an impulse to it
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>((hit.GetActor()->GetRootComponent()));
			if (mesh)
				mesh->AddRadialImpulse(Location, ImpactRadius, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, true);
			//or if the actor is destructible, activate that props radial force component (destructible meshes dont have a static mesh)
			ADestructibleProp* destructible = Cast<ADestructibleProp>(hit.GetActor());
			if (destructible)
			{
				destructible->FireRadialImpulse();
			}
		}
	}
	//destroy self after time has elapsed
	GetWorld()->GetTimerManager().SetTimer(DestroySelfTimer, this, &APhysicsProp::DestroySelf, SelfDestroyTime, false);
}

void APhysicsProp::DestroySelf()
{
	this->Destroy();
}

void APhysicsProp::AllowHit()
{
	canRegisterHit = true;
}
