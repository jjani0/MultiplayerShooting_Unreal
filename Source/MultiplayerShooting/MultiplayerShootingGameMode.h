// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerShootingGameMode.generated.h"

UCLASS()
class MULTIPLAYERSHOOTING_API AMultiplayerShootingGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
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
