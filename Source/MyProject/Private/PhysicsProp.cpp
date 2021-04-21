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

		GetWorld()->GetTimerManager().SetTimer(timer, this, &APhysicsProp::AllowHit, 0.5f, false);

	}
}

void APhysicsProp::AllowHit()
{
	canRegisterHit = true;
}

