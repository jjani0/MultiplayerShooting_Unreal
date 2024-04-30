// Fill out your copyright notice in the Description page of Project Settings.


#include "DoozyPlayerController.h"
#include "DoozyCharacter.h"
#include "DoozyMenu.h"
#include "Blueprint/UserWidget.h"

void ADoozyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController() || DoozyMenuClass == nullptr) { return; }
	DoozyMenu = CreateWidget<UDoozyMenu>(this, DoozyMenuClass);
	if (DoozyMenu != nullptr)
	{
		DoozyMenu->AddToViewport(0); // add menu when the play begins
	}
}

void ADoozyPlayerController::ToggleScoreboard()
{
	if (DoozyMenu != nullptr)
	{
		DoozyMenu->ToggleScoreboard();
	}
}

void ADoozyPlayerController::ClientNotifyKill_Implementation(const FString& Name)
{
	if (DoozyMenu != nullptr)
	{
		DoozyMenu->NotifyKill(Name);
	}
}

void ADoozyPlayerController::ClientShowScoreboard_Implementation()
{
	if (DoozyMenu != nullptr)
	{
		DoozyMenu->SetScoreboardVisibility(true);
	}
}