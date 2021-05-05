// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProp.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//--- Setting up base component
	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	RootComponent = Base;
	
	//--- Setting up the mesh
	PropMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("propMesh"));
	PropMesh->SetNotifyRigidBodyCollision(true);
	PropMesh->SetupAttachment(RootComponent);
	PropMesh->SetSimulatePhysics(true);
	
	//--- Setting up radial force
	Force = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce")); // minor force component to add some impulse on break
	Force->SetMobility(EComponentMobility::Movable);
	Force->SetupAttachment(GetRootComponent());
	Force->Radius = 100.0f;
	Force->ImpulseStrength = ImpulseForce;
	Force->bImpulseVelChange = true;
	Force->AddCollisionChannelToAffect(ECC_WorldDynamic);
	Force->SetRelativeLocation(FVector(0, 0, 0));
	Force->SetAutoActivate(false);
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

void ADestructibleProp::DestroyProp()
{
	this->Destroy();
}

void ADestructibleProp::FireRadialImpulse()
{
	Force->FireImpulse();
}
