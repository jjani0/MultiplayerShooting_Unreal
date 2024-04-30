// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MultiplayerShooting.h"
#include "EnumTypes.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "DoozyCharacter.generated.h"

struct FInputActionValue;

UCLASS(config = Game)
class MULTIPLAYERSHOOTING_API ADoozyCharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PistolAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RifleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShotgunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PreviousWeaponAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* NextWeaponAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScoreboardAction;

public:
	ADoozyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnPressedFire(); // firing weapon
	void OnReleasedFire();
	void OnPressedPistol(); // set pistol as a weapon
	void OnPressedRifle();
	void OnPressedShotgun();
	void OnPressedPreviousWeapon();
	void OnPressedNextWeapon();
	void OnPressedScoreboard();

	// Health & Armor - should be replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Doozy")
	float Health = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	float MaxHealth = 100.0f;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Doozy")
	float Armor = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	float MaxArmor = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	float ArmorAbsorption = 0.5;

	// Ammo & Weapons - should be replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Doozy")
	TArray<int32> Ammo;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Doozy")
	TArray<AWeapon*> Weapons; // array of pointers to INSTANCES
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	TArray<TSubclassOf<AWeapon>> WeaponClasses; // array of REFENCE TO CLASSES
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Doozy")
	AWeapon* Weapon; // reference to the currently equipped weapon
	int32 WeaponIndex = INDEX_NONE; // index of the currently equipped weapon

	// Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	USoundBase* SpawnSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	USoundBase* HitSound; // use this when hit other player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	USoundBase* WeaponChangeSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	USoundBase* PainSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doozy")
	USoundBase* LandSound;

	// Weapon RPC Functions
	UFUNCTION(Server, Reliable)
	void ServerCycleWeapons(int32 Direction);
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(EWeaponType WeaponType);

	// change the existing weapon to a new one
	bool EquipWeapon(EWeaponType WeaponType, bool bPlaySound = true);

	UPROPERTY()
	class AMultiplayerShootingGameMode* GameMode;

	// GameMode Functions
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void Landed(const FHitResult& Hit) override;

public:	
	virtual void Tick(float DeltaTime) override;

	//Camera
	FVector GetCameraLocation() const { return Camera->GetComponentLocation(); }
	FVector GetCameraDirection() const { return GetControlRotation().Vector(); }

	// Health
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health == 0.0f; }
	// Armor
	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	void ArmorAbsorbDamage(float& Damage);
	//Ammo and Weapon
	UFUNCTION(BlueprintCallable, Category = "Doozy")
	int32 GetWeaponAmmo() const { return (Weapon != nullptr) ? Ammo[ENUM_TO_INT32(Weapon->GetAmmoType())] : 0; }
	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); } // add
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); } // consume
	int32 GetAmmo(EAmmoType AmmoType) const { return Ammo[ENUM_TO_INT32(AmmoType)]; } // get
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); } // set
	void ApplyDamage(float Damage, ADoozyCharacter* DamageCauser);
	void AddWeapon(EWeaponType WeaponType); // get the new weapon

	// Anim & Sound RPC Functions
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAmimMontage(UAnimMontage* AnimMontage);
	UFUNCTION(Client, Unreliable) // make sound only on the owning client
	void ClientPlaySound2D(USoundBase* Sound); // use when character change the weapon (in the equipweapon function)

};
