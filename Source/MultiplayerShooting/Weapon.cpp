// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "DoozyCharacter.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // enable replication for this actor, if true, this actor will replicate to remote machines

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	Character = Cast<ADoozyCharacter>(NewOwner);
}

void AWeapon::StartFire() // check hitscan
{
	// Check whether firing is available
	if (!bWantsFire || GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.0f) { return; } // check if anim is still running
	if (Character->GetAmmo(AmmoType) == 0) // check remain ammo
	{
		if (NoAmmoSound != nullptr)
		{
			Character->ClientPlaySound2D(NoAmmoSound);
		}
		return;
	}

	Character->ConsumeAmmo(AmmoType, 1); // consume ammo

	const FVector FireLocation = Character->GetCameraLocation();
	const FVector FireDirection = Character->GetCameraDirection();
	FireHitscan(FireLocation, FireDirection); // and execute hitscan

	if (FireAnimMontage != nullptr)
	{
		// multicast RPC, it will play anim in all of the instances
		Character->MulticastPlayAmimMontage(FireAnimMontage);
	}
	if (FireMode == EWeaponFireMode::Automatic && bWantsFire)
	{
		//  schedule the timer to call function again after FireRate sec have passed
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::StartFire, FireRate);
	}
	else if (FireMode == EWeaponFireMode::Single)
	{
		// schedule timer with FireRate without a callback to just prevent player spamming the button
		GetWorldTimerManager().SetTimer(FireTimer, FireRate, false);
	}
}

void AWeapon::FireHitscan(FVector FireLocation, FVector FireDirection) // line trace
{
	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Fire Trace", false, Character); // ignores the owning charcter
	const FVector Start = FireLocation;
	const FVector End = Start + FireDirection * HitscanRange;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); // execute line trace

	// check whether the player shot other players (aim of the line trace)
	ADoozyCharacter* HitCharacter = Cast<ADoozyCharacter>(Hit.GetActor());
	if (HitCharacter != nullptr)
	{
		HitCharacter->ApplyDamage(HitscanDamage, Character);
	}
}

void AWeapon::ServerStartFire_Implementation()
{
	bWantsFire = true;
	StartFire();
}

void AWeapon::ServerStopFire_Implementation()
{
	bWantsFire = false;
}

void AWeapon::OnPressedFire()
{
	ServerStartFire();
}

void AWeapon::OnReleasedFire()
{
	ServerStopFire();
}