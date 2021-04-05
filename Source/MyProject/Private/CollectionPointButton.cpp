// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionPointButton.h"

#include "PackageCollectionPoint.h"
#include "Components/BillboardComponent.h"

// Sets default values
ACollectionPointButton::ACollectionPointButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<UStaticMeshComponent>(FName("Platform"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> platformMesh(TEXT("/Game/Assets/JoeAssets/ButtonStand/ButtonStand.ButtonStand"));

	base->SetStaticMesh(platformMesh.Object);

	RootComponent = base;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	boxComponent->SetWorldLocation(GetActorLocation());

	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectionPointButton::OnOverlapBegin);

	boxComponent->OnComponentEndOverlap.AddDynamic(this, &ACollectionPointButton::OnOverlapEnd);

	boxComponent->SetBoxExtent(FVector(70, 110, 110));
	boxComponent->SetRelativeLocation(FVector(0, -60, 40));

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> aButtonTex(TEXT("/Game/Assets/JoeAssets/XboxButtons/XboxOne_A.XboxOne_A"));

	BillboardComponent->Sprite = aButtonTex.Object;

	BillboardComponent->SetRelativeLocation({ 0,0,100 });

	BillboardComponent->SetHiddenInGame(false);
	
	BillboardComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectionPointButton::BeginPlay()
{
	Super::BeginPlay();

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

}

void ACollectionPointButton::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarhousePawn::StaticClass())) {
		auto player = reinterpret_cast<AWarhousePawn*>(OtherActor);
		if (CollidingPlayers.Contains(player)) return;
		CollidingPlayers.Add(player);
		player->InputComponent->BindAction("AButtonPressed", IE_Pressed, this, &ACollectionPointButton::AButtonPressed);
	}
}

void ACollectionPointButton::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AWarhousePawn::StaticClass())) {
		auto player = reinterpret_cast<AWarhousePawn*>(OtherActor);
		if (!CollidingPlayers.Contains(player)) return;
		CollidingPlayers.Remove(player);
		
		player->InputComponent->RemoveActionBinding("AButtonPressed", IE_Pressed);
	}
}

void ACollectionPointButton::AButtonPressed()
{
	packageCollectionPoint->ButtonPressed();
}