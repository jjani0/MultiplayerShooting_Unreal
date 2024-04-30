// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "DoozyCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(30.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	RootComponent = CollisionComp;
	//CollisionComp->SetCollisionProfileName("OverlapAllDynamic");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	//Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform); 
	//Mesh->SetCollisionProfileName("OverlapAll");

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.0f, 90.0f, 0.0f);

	bReplicates = true; // replicate actor to remote machines
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
}

void APickup::OnPickedUp(class ADoozyCharacter* Character)
{
	if (PickupSound != nullptr)
	{
		Character->ClientPlaySound2D(PickupSound);
	}
}

void APickup::SetInGameEnabled(bool NewbIsEnabled) // not hide and enable collision
{
	bIsEnabled = NewbIsEnabled;
	SetActorHiddenInGame(!bIsEnabled); // if the param is false, it is hidden
	SetActorEnableCollision(bIsEnabled);
}

void APickup::Respawn()
{
	SetInGameEnabled(true);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ADoozyCharacter* Character = Cast<ADoozyCharacter>(OtherActor);
	//if (Character == nullptr || Character->IsDead() || !HasAuthority()) { return; }
	if (Character == nullptr || Character->IsDead()) { return; }
	OnPickedUp(Character); // sound
	SetInGameEnabled(false); 
	// call Respawn func by RespawnTime
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &APickup::Respawn, RespawnTime); 
}