// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProp.h"

// Sets default values
APhysicsProp::APhysicsProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("packageMesh"));
	PropMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = PropMesh;
	//set health
	PropHealth = 100.0f;

}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();
	PropMesh->OnComponentHit.AddDynamic(this, &APhysicsProp::OnHit);

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
	ParticleSystem->ActivateSystem(true);
	//set timer to deactivate particle system
	GetWorld()->GetTimerManager().SetTimer(ParticlesTimerHandle, this, &APhysicsProp::DeactivateParticles, ParticleLife, false);
}

void APhysicsProp::DeactivateParticles()
{
	ParticleSystem->DeactivateSystem();
}

void APhysicsProp::DestroyProp()
{
	this->Destroy();
}

void APhysicsProp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		float velocity = this->GetVelocity().Size();
		if ((velocity > 1.0f) && (!isPropDead))
		{
			PropHealth -= 10.0f;
		}

	}

	if (GetUseParticleEmitter() && PropHealth < 0.0f)
	{
		PropHealth = 0.0f;
		isPropDead = true;
		ActivateParticles();
	}
}
