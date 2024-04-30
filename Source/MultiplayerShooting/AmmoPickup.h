// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "EnumTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class MULTIPLAYERSHOOTING_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoPickup")
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoPickup")
	int32 AmmoAmount;

	virtual void OnPickedUp(class ADoozyCharacter* Character) override;
};
