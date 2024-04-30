// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DoozyMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTING_API UDoozyMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent) // implement in BP
	void ToggleScoreboard();
	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreboardVisibility(bool bIsVisible);
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyKill(const FString& Name);
};
