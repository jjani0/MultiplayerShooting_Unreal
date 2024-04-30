// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerShootingGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "DoozyPlayerState.h"
#include "DoozyGameState.h"
#include "DoozyCharacter.h"
#include "DoozyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AMultiplayerShootingGameMode::AMultiplayerShootingGameMode()
{
	DefaultPawnClass = ADoozyCharacter::StaticClass();
	PlayerControllerClass = ADoozyPlayerController::StaticClass();
	PlayerStateClass = ADoozyPlayerState::StaticClass();
	GameStateClass = ADoozyGameState::StaticClass();
}

bool AMultiplayerShootingGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AMultiplayerShootingGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	ADoozyGameState* DoozyGameState = Cast<ADoozyGameState>(GameState);
	if (DoozyGameState != nullptr)
	{
		DoozyGameState->SetKillLimit(KillLimit);
	}
}

void AMultiplayerShootingGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, ADoozyPlayerController::StaticClass(), PlayerControllers);

	for (AActor* PlayerController : PlayerControllers)
	{
		ADoozyPlayerController* DoozyPlayerController = Cast<ADoozyPlayerController>(PlayerController);
		if (DoozyPlayerController == nullptr) continue;
		APawn* Pawn = DoozyPlayerController->GetPawn();
		if (Pawn != nullptr)
		{
			Pawn->Destroy();
		}
		DoozyPlayerController->ClientShowScoreboard();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerShootingGameMode::RestartMap, 5.0f);
}

bool AMultiplayerShootingGameMode::ReadyToEndMatch_Implementation()
{
	return HasWinner(); // check if there is a winner
}

bool AMultiplayerShootingGameMode::HasWinner() const
{
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		ADoozyPlayerState* DoozyPlayerState = Cast<ADoozyPlayerState>(PlayerState);
		if (DoozyPlayerState != nullptr && DoozyPlayerState->GetKills() == KillLimit)
		{
			return true;
		}
	}
	return false;
}

void AMultiplayerShootingGameMode::OnKill(AController* KillerController, AController* VictimController)
{
	if (!IsMatchInProgress()) { return; }

	if (KillerController != nullptr && KillerController != VictimController) // whthere there is a killer and not kill itself
	{
		ADoozyPlayerState* KillerDoozyState = Cast<ADoozyPlayerState>(KillerController->PlayerState);
		if (KillerDoozyState != nullptr)
		{
			KillerDoozyState->AddKill(); // add the kill to the killer
		}
		ADoozyPlayerController* KillerDoozyController = Cast<ADoozyPlayerController>(KillerController);
		if (KillerDoozyController != nullptr && VictimController != nullptr && VictimController->PlayerState != nullptr)
		{
			// notify the victim player to the killer client
			KillerDoozyController->ClientNotifyKill(VictimController->PlayerState->GetPlayerName());
		}
	}

	if (VictimController != nullptr)
	{
		ADoozyPlayerState* VictimPlayerState = Cast<ADoozyPlayerState>(VictimController->PlayerState);
		if (VictimPlayerState != nullptr)
		{
			VictimPlayerState->AddDeath(); // add death to the victim
		}
		APawn* VictimPawn = VictimController->GetPawn();
		if (VictimPawn != nullptr)
		{
			VictimPawn->Destroy();
		}
		if (!HasWinner())
		{
			RestartPlayer(VictimController); // and restart
		}
	}
}

void AMultiplayerShootingGameMode::RestartMap()
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false); // restart current map
}