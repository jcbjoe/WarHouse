// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorLight.h"

// Sets default values
ADoorLight::ADoorLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//--- Imports setup
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lightMeshRef(TEXT("/Game/Assets/ConorAssets/Door_Lights_pCylinder18.Door_Lights_pCylinder18"));

	//--- Light mesh setup
	lightMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Light"));
	lightMesh->SetStaticMesh(lightMeshRef.Object);
	RootComponent = lightMesh;

	//--- Light 1 setup
	light1 = CreateDefaultSubobject<USpotLightComponent>(FName("Light1"));
	light1->IntensityUnits = ELightUnits::Candelas;
	light1->Intensity = intensity;
	light1->AttenuationRadius = 90;
	light1->SetInnerConeAngle(0);
	light1->SetOuterConeAngle(44);
	light1->SetSourceRadius(0);
	light1->SetSoftSourceRadius(0);
	light1->SourceLength = 0;
	light1->LightColor = FColor::FromHex("270000FF");
	light1->SetRelativeRotation(FRotator::MakeFromEuler({ 0,light1Rotation,90 }));
	light1->SetRelativeLocation({ 2.5,0,0 });
	light1->SetupAttachment(RootComponent);

	//--- Light 2 setup
	light2 = CreateDefaultSubobject<USpotLightComponent>(FName("Light2"));
	light2->IntensityUnits = ELightUnits::Candelas;
	light2->Intensity = intensity;
	light2->AttenuationRadius = 90;
	light2->SetInnerConeAngle(0);
	light2->SetOuterConeAngle(44);
	light2->SetSourceRadius(0);
	light2->SetSoftSourceRadius(0);
	light2->SourceLength = 0;
	light2->LightColor = FColor::FromHex("270000FF");
	light2->SetRelativeRotation(FRotator::MakeFromEuler({ 0,light2Rotation,90 }));
	light2->SetRelativeLocation({ 2.5,0,0 });
	light2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorLight::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoorLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(isSpinning)
	{
		light1->SetIntensity(intensity);
		light2->SetIntensity(intensity);
		
		light1Rotation += 1;
		light2Rotation += 1;
		
		light1->SetRelativeRotation(FRotator::MakeFromEuler({ 0,light1Rotation,90 }));
		light2->SetRelativeRotation(FRotator::MakeFromEuler({ 0,light2Rotation,90 }));
	} else
	{
		light1->SetIntensity(0);
		light2->SetIntensity(0);
	}
}

void ADoorLight::SetSpinning(bool val)
{
	isSpinning = val;
}

