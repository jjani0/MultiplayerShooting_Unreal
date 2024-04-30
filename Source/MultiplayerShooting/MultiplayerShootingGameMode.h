// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerShootingGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerShootingGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	bool HasWinner() const;
	void OnKill(AController* KillerController, AController* VictimController);

protected:
	AMultiplayerShootingGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer Shooting Game Mode")
	int32 KillLimit = 30;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	void RestartMap();
};



