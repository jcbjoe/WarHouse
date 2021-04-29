// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProp.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up mesh
	PropMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("propMesh"));
	PropMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = PropMesh;
	PropMesh->SetSimulatePhysics(true);
	//set radial impact data
	ImpactRadius = 200.0f;
	RadialImpactForce = 2000.0f;

}

// Called when the game starts or when spawned
void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADestructibleProp::GetUseParticleEmitter()
{
	return UseParticleEmitter;
}

bool ADestructibleProp::GetCanExplode()
{
	return CanExplode;
}


void ADestructibleProp::ActivateParticles()
{
	ParticleSystem->ActivateSystem(true);
	//set timer to deactivate particle system
	GetWorld()->GetTimerManager().SetTimer(ParticlesTimerHandle, this, &ADestructibleProp::DeactivateParticles, ParticleLife, false);
}

void ADestructibleProp::DeactivateParticles()
{
	ParticleSystem->DeactivateSystem();
}

void ADestructibleProp::DestroyProp()
{
	this->Destroy();
}

void ADestructibleProp::Explode()
{
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

//void ADestructibleProp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
//	{
//		float velocity = this->GetVelocity().Size();
//		if ((velocity > 1.0f) && (!isPropDead))
//		{
//			if (IsFragile)
//				PropHealth -= PropHealth;
//			else
//				PropHealth -= 10.0f;
//		}
//
//	}
//
//	if (GetUseParticleEmitter() && PropHealth < 0.0f)
//	{
//		PropHealth = 0.0f;
//		isPropDead = true;
//		ActivateParticles();
//	}
//
//}
