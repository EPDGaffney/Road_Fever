// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Public/Items/Item.h"
#include "Public/Items/Ammo/Ammo.h"
#include "Weapon.generated.h"

// The properties to customize how the weapon behaves [20/11/2015 Matthew Woolley]
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

	// Whether the weapons is cooling down or not [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Attack" )
	bool bIsCoolingDown;

	// Whether this weapon is reloading or now. [27/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Attack" )
	bool bIsReloading;

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
	bool bIsBoxTrace;

	// Played when the player uses the attack function, with ammo in the weapon if required. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* AttackAnimation;

	// Played when the player uses the attack function, without ammo in the weapon. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* AttackAnimation_NoAmmo;

	// Played when the player uses the reload function. [11/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation" )
	UAnimationAsset* ReloadAnimation;

};



/**
 * Base class for all weapons
 */
UCLASS( abstract, Blueprintable, BlueprintType )
class ROAD_FEVER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Called when this Weapon enters memory [20/11/2015 Matthew Woolley]
	AWeapon();

	// Called when this object is destroyed. [27/7/2016 Matthew Woolley]
	virtual void EndPlay( const EEndPlayReason::Type InEndPlayReason ) override;

	// Called when the player wishes to attack with this weapon [20/11/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attack" )
	void OnAttack();
	virtual void OnAttack_Implementation();

	// The properties for customizing this weapon [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	FWeaponProperties WeaponProperties;

	// Called when the user wishes to reload; bShouldUseFullClip will be true if they don't hold the reload key. [25/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void Reload( bool bUseFullClip );

	// Called when the weapon has cooled down. [27/7/2016 Matthew Woolley]
	void Cooldown();

	// Called when the weapon used is using a clip and requires a "full-reload" each time. [27/7/2016 Matthew Woolley]
	void FullReload();

	// Called when the weapon used is loaded one-shot at a time. [27/7/2016 Matthew Woolley]
	void SingleRoundReload();

	// Whether or not the reload should attempt a full clip. [27/7/2016 Matthew Woolley]
	bool bShouldUseFullClip;

	// Whether or not the reloading is getting interrupted. [27/7/2016 Matthew Woolley]
	bool bShouldInterrupt;

private:
	// The timer for cooling the weapon down. [27/7/2016 Matthew Woolley]
	FTimerHandle WeaponCooldownHandle;

	// The timer for reloading the weapon. [27/7/2016 Matthew Woolley]
	FTimerHandle WeaponReloadHandle;

	// The item created when doing reload checks. [27/7/2016 Matthew Woolley]
	class AItem* TemporaryItemInfoHolder;
};
