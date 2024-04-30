// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "DoozyCharacter.h"

void AHealthPickup::OnPickedUp(ADoozyCharacter* Character)
{
	Super::OnPickedUp(Character);
	Character->AddHealth(HealAmount);
}
