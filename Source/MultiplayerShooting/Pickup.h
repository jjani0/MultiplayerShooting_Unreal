// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MULTIPLAYERSHOOTING_API APickup : public AActor
{
	GENERATED_BODY()

protected:
	APickup();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class URotatingMovementComponent* RotatingMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	float RespawnTime = 30.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	USoundBase* PickupSound;

	// myadd
//UPROPERTY(VisibleDefaultsOnly, Category = "Pickup")
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class USphereComponent* CollisionComp;

	FTimerHandle RespawnTimer;
	bool bIsEnabled = true; // whether the pickup is enabled, use when respawn this actor

	virtual void OnPickedUp(class ADoozyCharacter* Character);
	void SetInGameEnabled(bool NewbIsEnabled); // not hide and enable collision
	void Respawn();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Hit);
};
