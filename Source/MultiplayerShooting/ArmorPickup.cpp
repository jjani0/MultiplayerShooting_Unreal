// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorPickup.h"
#include "DoozyCharacter.h"

void AArmorPickup::OnPickedUp(ADoozyCharacter* Character)
{
	Super::OnPickedUp(Character);
	Character->AddArmor(ArmorAmount);
}