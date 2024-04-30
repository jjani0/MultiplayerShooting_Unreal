// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DoozyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTING_API ADoozyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy Plyaer Controller")
	TSubclassOf<class UDoozyMenu> DoozyMenuClass; // will use the player menu
	UPROPERTY()
	class UDoozyMenu* DoozyMenu;

	virtual void BeginPlay() override;

public:
	void ToggleScoreboard();

	// RPC Inform Functions
	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);
	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard();
};
