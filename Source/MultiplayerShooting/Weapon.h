// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.h"
#include "Animation/AnimMontage.h"
#include "Weapon.generated.h"

UCLASS()
class MULTIPLAYERSHOOTING_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	AWeapon();
	virtual void BeginPlay() override;

	// Weapon Information
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EAmmoType AmmoType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponFireMode FireMode;

	// Hitscan
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HitscanRange = 9999.9f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HitscanDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate = 1.0f; // amount of time to fire again

	// Anim & Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimMontage; // excute this anim montage while firing 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundBase* NoAmmoSound;

	// Handling Fire
	FTimerHandle FireTimer;  // a timer to prevent firing while playing the fire animation
	bool bWantsFire; 
	class ADoozyCharacter* Character; // character reference
	virtual void SetOwner(AActor* NewOwner) override; // to set the character
	void StartFire();
	void FireHitscan(FVector FireLocation, FVector FireDirection);

	// RPC Functions
	UFUNCTION(Server, Reliable)
	void ServerStartFire();
	UFUNCTION(Server, Reliable)
	void ServerStopFire();

public:	
	EAmmoType GetAmmoType() const { return AmmoType; }

	// Weapon Fire Functions
	virtual void OnPressedFire();
	virtual void OnReleasedFire();
};
