// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProp.h"
#include "DrawDebugHelpers.h"
#include "WarhousePawn.h"
#include "DestructibleComponent.h"
#include "DestructibleProp.h"
// Sets default values
APhysicsProp::APhysicsProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	PropMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	PropMeshComponent->SetNotifyRigidBodyCollision(true);
	//set health
	PropHealth = 100.0f;
	//simulate physics
	PropMeshComponent->SetSimulatePhysics(true);
	PropMeshComponent->SetMassOverrideInKg(NAME_None, 20);
	RootComponent = PropMeshComponent;
	//set up particles
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleEmitter"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetVisibility(false);
	//set radial impact data
	ImpactRadius = 500.0f;
	RadialImpactForce = 2000.0f;
	//set up audio
	static ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("/Game/Extras/Audio/Explosion_Cue.Explosion_Cue"));
	soundBase = sound.Object;
}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystemComponent->DeactivateSystem();
	PropMeshComponent->OnComponentHit.AddDynamic(this, &APhysicsProp::OnHit);

	//check properties to see what behaviour should be allowed
	if (IsFragile)
	{
		PropHealth = 1.0f;
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
	//ParticleSystemComponent->Activate();

	ParticleSystemComponent->SetVisibility(true);
	ParticleSystemComponent->ActivateSystem();
	//set timer to deactivate particle system
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
		auto velocity = this->GetVelocity().Size();

		if (velocity > 105 && !isPropDead && canRegisterHit)
		{
			PropHealth -= 5.0f;
		}

		if (GetUseParticleEmitter() && PropHealth < 0.0f)
		{
			PropHealth = 0.0f;
			isPropDead = true;
			ActivateParticles();
		}

		if (GetCanExplode() && PropHealth < 0.0f)
		{
			PropHealth = 0.0f;
			isPropDead = true;
			if (ParticleSystemComponent)
				ActivateParticles();
			Explode();
		}

		GetWorld()->GetTimerManager().SetTimer(Timer, this, &APhysicsProp::AllowHit, 0.5f, false);

	}
}

void APhysicsProp::Explode()
{
	//play sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundBase, this->GetActorLocation(), 1.0f);
	//activate particles
	ActivateParticles();
	//radial impulse
	FVector Location = this->GetActorLocation();
	FCollisionShape SphereCol = FCollisionShape::MakeSphere(ImpactRadius);
	//DrawDebugSphere(GetWorld(), this->GetActorLocation(), SphereCol.GetSphereRadius(), 50, FColor::Emerald, true);
	bool SweepHit = GetWorld()->SweepMultiByChannel(HitActors, Location, Location, FQuat::Identity, ECC_WorldStatic, SphereCol);
	if (SweepHit)
	{
		for (auto& hit : HitActors)
		{
			//check if its a player and kill them
			if (hit.GetActor()->IsA(AWarhousePawn::StaticClass()))
			{
				AWarhousePawn* player = Cast<AWarhousePawn>(hit.GetActor());
				player->KillPlayer();
			}
			//get the mesh and apply force
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>((hit.GetActor()->GetRootComponent()));
			if (mesh)
				mesh->AddRadialImpulse(Location, ImpactRadius, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, true);
			//get destructible and apply force
			ADestructibleProp* destructible = Cast<ADestructibleProp>(hit.GetActor());
			if (destructible)
			{
				destructible->FireRadialImpulse();
			}
		}
	}
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
