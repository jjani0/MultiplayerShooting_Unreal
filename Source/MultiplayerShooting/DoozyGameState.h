// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DoozyGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTING_API ADoozyGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS GameState")
	int32 KillLimit;

public:
	UFUNCTION(BlueprintCallable)
	TArray<class ADoozyPlayerState*> GetPlayerStatesOrderedByKills() const;

	void SetKillLimit(int32 NewKillLimit) { KillLimit = NewKillLimit; } // used by gamemode 
};
