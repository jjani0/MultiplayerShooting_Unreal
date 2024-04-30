// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DoozyPlayerState.generated.h"

UCLASS()
class MULTIPLAYERSHOOTING_API ADoozyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS PlayerState")
	int32 Kills;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS PlayerState")
	int32 Deaths;

public:
	void AddKill() { Kills++; }
	void AddDeath() { Deaths++; }
	int32 GetKills() const { return Kills; }
};
