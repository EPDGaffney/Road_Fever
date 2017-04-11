// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "WeaponProperties.generated.h"



/*
 *	Used by the weapon code to determine whether or not the weapon is available to attack or not.
 *	11/4/2017 - Matthew Woolley
 */
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	Reloading,
	CoolingDown,
	Normal
};



// The properties to customize how the weapon behaves [20/11/2015 Matthew Woolley]
/*
 *	Used to customise weapon behaviour, such as cool down length, damage etc..
 *	11/4/2017 - Matthew Woolley
 */
USTRUCT( BlueprintType )
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()

public:
	// The maximum amount of damage this weapon does at effective range. [17/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 EffectiveRangeMaxDamage;

	// The minimum amount of damage this weapon does at effective range. [18/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 EffectiveRangeMinDamage;

	// The maximum amount of damage (taken away from the base damage) this weapon does at maximum range. [18/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 MaximumRangeMaxDamage;

	// The minimum amount of damage (taken away from the base damage) this weapon does at maximum range. [18/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 MaximumRangeMinDamage;

	// The range that does full damage (In CM). [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	float EffectiveRange;

	// The range of which this weapon will still deal damage (In CM). [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	float MaximumRange;

	// The time between each attack [20/11/2015 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	float CoolDownTime;

	// This time it takes to reload this weapon (per round for fed weapons, or per clip for clipped weapons). [27/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	float ReloadTime;

	// The amount of traces this weapon does per shot. [21/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	int32 NumberOfTraces;

	// How much each line trace should trail off from the center of the gun (first shot will be centered). [21/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	float MultiTraceSpread;

	// The kind of ammo this weapon uses. [25/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	TSubclassOf<AAmmo> AmmoType;

	// Whether this weapon uses a line trace, or a box trace. [3/2/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	bool bBoxTrace;

	// Played when the player uses the attack function, with ammo in the weapon if required. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* AttackAnimation;

	// Played when the player uses the attack function, without ammo in the weapon. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* AttackAnimation_NoAmmo;

	// Played when the player uses the reload function. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* ReloadAnimation;

	// The current state of the weapon. [19/3/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	EWeaponState WeaponState;
};