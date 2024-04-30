// Fill out your copyright notice in the Description page of Project Settings.


#include "DoozyPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADoozyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoozyPlayerState, Kills);
	DOREPLIFETIME(ADoozyPlayerState, Deaths);
}