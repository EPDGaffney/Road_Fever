// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"



/*
 * The struct that controls default info for elements of the inventory (inventory slots)
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
	bool bIsEquipable;

	// Whether or not this item is equipped. [13/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Equipping" )
	bool bIsEquipped;

	// Whether or not this item is a weapon. [24/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	bool bIsWeapon;

	// The maximum amount of times this weapon can be used before reloading (if it is a weapon). [24/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Attack" )
	int32 MaxAmmo;

	// The current amount of ammo in this weapon (if it is a weapon) or contained in this pickup (if it is an ammo pickup). [24/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	int32 CurrentAmmo;

	// Whether this ammo type is a clip. [25/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	bool bIsClip;
};




/**
	* Base class for all items
	* Use this class for everything that the player can use and equip
	* This class has no native functionality and, therefore, is a conduit for child classes
*/
UCLASS()
class ROAD_FEVER_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Called when this Item is created. [14/7/2016 Matthew Woolley]
	AItem();

	// Called when the user wishes to use an item. [21/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnUse();
	virtual void OnUse_Implementation() { return; };

	// Called when the user wishes to combine an item. [21/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnCombine();
	virtual void OnCombine_Implementation() { return; };

	// Called when the user wishes to examine an item. [21/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnExamine();
	virtual void OnExamine_Implementation() { return; };

	// Called when the user wishes to interact with an item. [21/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnInteract();
	virtual void OnInteract_Implementation() { return; };

	// Attaches the item to Ned's hand. [13/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Item" )
	void AttachItemToNed_Hand();

	// The info that will get passed to the inventory to find out what item has been picked up. [4/3/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	FInventoryItem ItemInfo;

	// The mesh that is used to represent this Item. [14/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	UStaticMeshComponent* ItemsMesh;

	// The animations that should be used when the character is not aiming. [18/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	TSubclassOf<UAnimInstance> AnimationBlueprint_Default;

	// The animations that should be used when the character is aiming. [18/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	TSubclassOf<UAnimInstance> AnimationBlueprint_Aiming;

	// Whether or not this item can be picked up (if it is equipped, the response is false). [20/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	bool bCanBePickedUp;

protected:
	// The box that the Character uses to detect this item on the ground. [14/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Item" )
	UBoxComponent* DetectionBox;

private:
	// Used to point the forward direction of the item. [14/7/2016 Matthew Woolley]
	UArrowComponent* DirectionArrow;
};
