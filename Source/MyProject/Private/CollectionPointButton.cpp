// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionPointButton.h"

#include "PackageCollectionPoint.h"
#include "SettingsSave.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectionPointButton::ACollectionPointButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<UStaticMeshComponent>(FName("Platform"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> buttonMesh(TEXT("/Game/Assets/ConorAssets/DeliveryLift/LiftButton/LiftButton.LiftButton"));

	base->SetStaticMesh(buttonMesh.Object);

	RootComponent = base;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	boxComponent->SetWorldLocation(GetActorLocation());

	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectionPointButton::OnOverlapBegin);

	boxComponent->OnComponentEndOverlap.AddDynamic(this, &ACollectionPointButton::OnOverlapEnd);

	boxComponent->SetBoxExtent(FVector(120, 120, 120));
	boxComponent->SetRelativeLocation(FVector(0, 0, 120));

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> aButtonTex(TEXT("/Game/Assets/JoeAssets/XboxButtons/XboxOne_A.XboxOne_A"));

	BillboardComponent->Sprite = aButtonTex.Object;

	BillboardComponent->SetRelativeLocation({ 0,0,160 });

	BillboardComponent->SetHiddenInGame(true);
	
	BillboardComponent->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<USoundWave> buttonPressSound(TEXT("/Game/Sounds/Robotic_scifi_SFX/Guns/wav/special_gun__2_.special_gun__2_"));

	audioComp = CreateDefaultSubobject<UAudioComponent>(FName("ButtonAudio"));

	audioComp->SetVolumeMultiplier(0.0f);
	
	audioComp->SetSound(buttonPressSound.Object);
}

// Called when the game starts or when spawned
void ACollectionPointButton::BeginPlay()
{
	Super::BeginPlay();

	if (USettingsSave* LoadedGame = Cast<USettingsSave>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		volumeMultiplier = LoadedGame->SFXVolume;
	}
}

// Called every frame
void ACollectionPointButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(packageCollectionPoint->IsHidden())
	{
		SetActorHiddenInGame(true);
	} else
	{
		SetActorHiddenInGame(false);
	}

	if(CollidingPlayers.Num() > 0)
	{
		BillboardComponent->SetHiddenInGame(false);
	} else
	{
		BillboardComponent->SetHiddenInGame(true);
	}

}

void ACollectionPointButton::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass())) {
		auto player = Cast<AWarhousePawn>(OtherActor);
		if (CollidingPlayers.Contains(player)) return;
		CollidingPlayers.Add(player);
		player->InputComponent->BindAction("AButtonPressed", IE_Pressed, this, &ACollectionPointButton::AButtonPressed);
	}
}

void ACollectionPointButton::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AWarhousePawn::StaticClass())) {
		auto player = Cast<AWarhousePawn>(OtherActor);
		if (!CollidingPlayers.Contains(player)) return;
		CollidingPlayers.Remove(player);
		
		player->InputComponent->RemoveActionBinding("AButtonPressed", IE_Pressed);
	}
}

void ACollectionPointButton::AButtonPressed()
{
	audioComp->SetVolumeMultiplier(buttonPushSoundMultiplier * volumeMultiplier);
	audioComp->Play();
	packageCollectionPoint->ButtonPressed();
}