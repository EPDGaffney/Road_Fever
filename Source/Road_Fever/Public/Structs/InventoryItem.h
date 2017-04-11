// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "InventoryItem.generated.h"



/*
 *	The default pieces of info that each item and slot within an inventory needs.
 *	11/4/2017 - Matthew Woolley
 */
USTRUCT()
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	// The class that gets spawned when "Use", or some other variant, has been clicked. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Item" )
	TSubclassOf<class AItem> ItemClass;

	// The texture that gets shown in the inventory [26/11/2015 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Item" )
	UTexture2D* DisplayIcon;

	// The name that the item has [26/11/2015 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Item" )
	FString DisplayName;

	// The text that accompanies this item, once it has been clicked. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Item" )
	FString ItemToolTip;

	// The maximum amount of this item that can fit in one slot. [4/3/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Item" )
	int32 MaxItemStack;

	// The amount of this item that is currently in this slot. [4/3/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	int32 CurrentItemStack;

	// Whether or not the item in this slot can be equipped. [13/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Equipping" )
	bool bEquipable;

	// Whether or not this item is equipped. [13/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Equipping" )
	bool bEquipped;

	// Whether or not this item is a weapon. [24/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	bool bWeapon;

	// The maximum amount of times this weapon can be used before reloading (if it is a weapon). [24/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 MaxAmmo;

	// The current amount of ammo in this weapon (if it is a weapon) or contained in this pickup (if it is an ammo pickup). [24/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	int32 CurrentAmmo;

	// Whether this ammo type is a clip. [25/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	bool bClip;
};