// Fill out your copyright notice in the Description page of Project Settings.


#include "DoozyGameState.h"
#include "Net/UnrealNetwork.h"
#include "DoozyPlayerState.h"

void ADoozyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoozyGameState, KillLimit);
}

TArray<ADoozyPlayerState*> ADoozyGameState::GetPlayerStatesOrderedByKills() const
{
	TArray<ADoozyPlayerState*> PlayerStates;
	for (APlayerState* PlayerState : PlayerArray)
	{
		ADoozyPlayerState* DoozyPlayerState = Cast<ADoozyPlayerState>(PlayerState);
		PlayerStates.Add(DoozyPlayerState);
	}
	PlayerStates.Sort([](const ADoozyPlayerState& A, const ADoozyPlayerState& B) { return A.GetKills() > B.GetKills(); });
	return PlayerStates;
}