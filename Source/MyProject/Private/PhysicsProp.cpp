// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProp.h"

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

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleEmitter"));

	ParticleSystemComponent->SetupAttachment(RootComponent);

	ParticleSystemComponent->SetVisibility(false);
	//set radial impact data
	ImpactRadius = 200.0f;
	RadialImpactForce = 2000.0f;
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

		GetWorld()->GetTimerManager().SetTimer(timer, this, &APhysicsProp::AllowHit, 0.5f, false);

	}
}

void APhysicsProp::Explode()
{
	//activate particles
	// 
	//radial impulse
	FHitResult Hit;
	FCollisionShape SphereCol = FCollisionShape::MakeSphere(ImpactRadius);
	bool SweepHit = GetWorld()->SweepMultiByChannel(HitActors, Hit.Location, Hit.Location + FVector(0.1f, 0.1f, 0.1f), FQuat::Identity, ECC_WorldStatic, SphereCol);
	if (SweepHit)
	{
		for (auto& hit : HitActors)
		{
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if (mesh)
				mesh->AddRadialImpulse(Hit.Location, ImpactRadius, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, false);
		}
	}
}

void APhysicsProp::AllowHit()
{
	canRegisterHit = true;
}
