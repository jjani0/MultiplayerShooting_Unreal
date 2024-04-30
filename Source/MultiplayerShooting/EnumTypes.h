#pragma once

// to access specific weapon type more easily
UENUM()
enum class EWeaponType : uint8
{
	Pistol,
	Rifle,
	Shotgun,
	MAX
};

UENUM()
enum class EWeaponFireMode : uint8
{
	Single,
	Automatic
};

UENUM()
enum class EAmmoType : uint8
{
	PistolBullets,
	RifleBullets,
	ShotgunSlugs,
	MAX
};