// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProp.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

bool ADestructibleProp::GetCanPickUp()
{
	return CanPickUp;
}

bool ADestructibleProp::GetUseParticleEmitter()
{
	return UseParticleEmitter;
}

bool ADestructibleProp::GetIsFragile()
{
	return IsFragile;
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

void ADestructibleProp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		float velocity = this->GetVelocity().Size();
		if ((velocity > 1.0f) && (!isPropDead))
		{
			if (IsFragile)
				PropHealth -= PropHealth;
			else
				PropHealth -= 10.0f;
		}

	}

	if (GetUseParticleEmitter() && PropHealth < 0.0f)
	{
		PropHealth = 0.0f;
		isPropDead = true;
		ActivateParticles();
	}

	if (isPropDead)
	{
		UGameplayStatics::ApplyPointDamage(this, 1.0f, OtherActor->GetActorForwardVector(), Hit, GetInstigatorController(), OtherActor, UDamageType::StaticClass());

	}
}
