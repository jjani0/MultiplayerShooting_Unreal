// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "DoozyCharacter.h"

void AAmmoPickup::OnPickedUp(class ADoozyCharacter* Character)
{
	Super::OnPickedUp(Character);
	Character->AddAmmo(AmmoType, AmmoAmount);
}