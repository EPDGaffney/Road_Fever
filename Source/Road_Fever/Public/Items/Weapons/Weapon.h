// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Public/Items/Item.h"
#include "Public/Items/Ammo/Ammo.h"
#include "Public/Structs/WeaponProperties.h"
#include "Weapon.generated.h"



/**
 * Base class for all weapons
 */
UCLASS( abstract, Blueprintable, BlueprintType )
class ROAD_FEVER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	/*
	*	Sets up default WeaponProperties for all new weapons (to be altered in Blueprint for each weapon).
	*	Setup default ItemInfo for use within the inventory and world.
	*	Allow the Tick function to fire every frame.
	*	11/4/2017 - Matthew Woolley
	*/
	AWeapon();

	/*
	*	Clears timers before this weapon is removed from memory.
	*	11/4/2017 - Matthew Woolley
	*/
	virtual void EndPlay( const EEndPlayReason::Type InEndPlayReason ) override;

	/*
	*	If the weapon is able to fire:
	*		play a shooting animation;
	*		create a line-trace for each trace and slightly angle it;
	*		If an enemy was found, deal damage to it.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attack" )
	void OnAttack();
	virtual void OnAttack_Implementation();

	/*
	*	Conduit function to determine whether to call FullReload or SingleRoundReload.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void Reload( bool bInUseFullClip );
	
	/*
	*	This is called for weapons that use a clip and require a full reload each time a reload has started.
	*	11/4/2017 - Matthew Woolley
	*/
	void FullReload();

	/*
	*	This is called for weapons that use single rounds to reload (such as a shotgun) and that can be interupted.
	*	11/4/2017 - Matthew Woolley
	*/
	void SingleRoundReload();

	/*
	*	Changes the weapon state so that the weapon can now fire normally.
	*	11/4/2017 - Matthew Woolley
	*/
	void Cooldown();

	// The properties for customizing this weapon [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	FWeaponProperties WeaponProperties;

	// Whether or not the reload should attempt a full clip. [27/7/2016 Matthew Woolley]
	bool bUseFullClip;

	// Whether or not the reloading is getting interrupted. [27/7/2016 Matthew Woolley]
	bool bInterruptReload;

private:
	// The timer for cooling the weapon down. [27/7/2016 Matthew Woolley]
	FTimerHandle WeaponCooldownHandle;

	// The timer for reloading the weapon. [27/7/2016 Matthew Woolley]
	FTimerHandle WeaponReloadHandle;

	// The item created when doing reload checks. [27/7/2016 Matthew Woolley]
	class AItem* TemporaryItemInfoHolder;
};
