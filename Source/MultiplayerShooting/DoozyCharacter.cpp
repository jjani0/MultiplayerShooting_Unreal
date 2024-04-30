// Fill out your copyright notice in the Description page of Project Settings.


#include "DoozyCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MultiplayerShootingGameMode.h"
#include "DoozyPlayerController.h"
#include "Weapon.h"


ADoozyCharacter::ADoozyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ADoozyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Initialize settings
	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
	//if (!HasAuthority()) { return; } // does not make client move

	SetHealth(MaxHealth); // initialize when the game starts

	// Important! Add Input Mapping Contex, cannot move without this code
	// set up enhanced input for player controller
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = \
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	const int32 WeaponCount = ENUM_TO_INT32(EWeaponType::MAX); // num of weapons
	Weapons.Init(nullptr, WeaponCount); // initialize weapons array
	const int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);
	Ammo.Init(50, AmmoCount); // initialize ammo array
	for (int32 i = 0; i < WeaponCount; i++)
	{
		AddWeapon((EWeaponType)i); // add weapon to the 'weapons' array
	}
	EquipWeapon(EWeaponType::Pistol, false); // default is pistol

	GameMode = Cast<AMultiplayerShootingGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ADoozyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
}

// Called to bind functionality to input
void ADoozyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADoozyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADoozyCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ADoozyCharacter::OnReleasedFire);
		EnhancedInputComponent->BindAction(PistolAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedPistol);
		EnhancedInputComponent->BindAction(RifleAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedRifle);
		EnhancedInputComponent->BindAction(ShotgunAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedShotgun);
		EnhancedInputComponent->BindAction(PreviousWeaponAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedPreviousWeapon);
		EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedNextWeapon);
		EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ADoozyCharacter::OnPressedScoreboard);
	}
}

void ADoozyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// always think about the 2D, 3D (xyz: forward/right/up) coord 
		// Movement is a 2D Vector
		AddMovementInput(ForwardDirection, MovementVector.Y); // character can move bc of the addmovement builtin func
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADoozyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADoozyCharacter::OnPressedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnPressedFire();
	}
}

void ADoozyCharacter::OnReleasedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnReleasedFire();
	}
}

void ADoozyCharacter::OnPressedPistol() { ServerEquipWeapon(EWeaponType::Pistol); }
void ADoozyCharacter::OnPressedRifle() { ServerEquipWeapon(EWeaponType::Rifle); }
void ADoozyCharacter::OnPressedShotgun() { ServerEquipWeapon(EWeaponType::Shotgun); }
void ADoozyCharacter::OnPressedPreviousWeapon() { ServerCycleWeapons(-1); }
void ADoozyCharacter::OnPressedNextWeapon() { ServerCycleWeapons(1); }

void ADoozyCharacter::OnPressedScoreboard()
{
	ADoozyPlayerController* PlayerController = Cast<ADoozyPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ToggleScoreboard();
	}
}

void ADoozyCharacter::GetLifetimeReplicatedProps(TArray < FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ADoozyCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADoozyCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADoozyCharacter, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADoozyCharacter, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADoozyCharacter, Ammo, COND_OwnerOnly);
}

void ADoozyCharacter::ArmorAbsorbDamage(float& Damage)
{
	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;
	SetArmor(RemainingArmor);
	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
}

void ADoozyCharacter::ApplyDamage(float Damage, ADoozyCharacter* DamageCauser)
{
	ArmorAbsorbDamage(Damage); // reduce armor
	RemoveHealth(Damage);

	if (HitSound != nullptr && DamageCauser != nullptr)
	{
		DamageCauser->ClientPlaySound2D(HitSound);
	}

	if (IsDead())
	{
		if (GameMode != nullptr)
		{
			GameMode->OnKill(DamageCauser->GetController(), GetController());
		}
	}
	else
	{
		ClientPlaySound2D(PainSound);
	}
}

void ADoozyCharacter::AddWeapon(EWeaponType WeaponType) 
{
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);
	// isvalidinex: just compare with the lenth of weaponclasses
	if (!WeaponClasses.IsValidIndex(NewWeaponIndex) || Weapons[NewWeaponIndex] != nullptr) { return; }

	UClass* WeaponClass = WeaponClasses[NewWeaponIndex]; // returns reference of the class
	if (WeaponClass == nullptr) { return; }
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters); // spawn newweapon
	if (NewWeapon == nullptr) { return; }
	// hide in game and attach weapon to the player socket
	NewWeapon->SetActorHiddenInGame(true);
	Weapons[NewWeaponIndex] = NewWeapon; // add on a weapon array which player possess
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GripPoint");
}

// equip or change the existing weapon to a new one
bool ADoozyCharacter::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);
	if (!Weapons.IsValidIndex(NewWeaponIndex)) { return false; }
	AWeapon* NewWeapon = Weapons[NewWeaponIndex];
	if (NewWeapon == nullptr || Weapon == NewWeapon) { return false; }
	// unequip the current weapon and equip the new weapon
	if (Weapon != nullptr)
	{
		Weapon->SetActorHiddenInGame(true); // set the current weapon hidden 
	}
	Weapon = NewWeapon;
	WeaponIndex = NewWeaponIndex;
	Weapon->SetActorHiddenInGame(false); // spawn new weapon
	if (WeaponChangeSound != nullptr && bPlaySound)
	{
		ClientPlaySound2D(WeaponChangeSound);
	}
	return true;
}

void ADoozyCharacter::ServerCycleWeapons_Implementation(int32 Direction)
{
	const int32 WeaponCount = Weapons.Num();
	const int32 StartWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);
	for (int32 i = StartWeaponIndex; i != WeaponIndex;
		i = GET_CIRCULAR_ARRAY_INDEX(i + Direction, WeaponCount)) // get the possible weapon the player got
	{
		if (EquipWeapon((EWeaponType)i)) { break; } // abort if the equip was successful
	}
}

void ADoozyCharacter::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	EquipWeapon(WeaponType);
}

void ADoozyCharacter::MulticastPlayAmimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void ADoozyCharacter::ClientPlaySound2D_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void ADoozyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (AWeapon* WeaponToDestroy : Weapons)
	{
		WeaponToDestroy->Destroy();
	}
}

void ADoozyCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	if (GameMode != nullptr)
	{
		GameMode->OnKill(nullptr, GetController());
	}
}

void ADoozyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}