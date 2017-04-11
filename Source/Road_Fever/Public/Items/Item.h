// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Structs/InventoryItem.h"
#include "Public/Structs/Crafting.h"
#include "Item.generated.h"



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
	/*
	*	Creates a box that is used by the character so that the character can detect the item.
	*	Creates a StaticMeshComponent that stores the mesh of the item.
	*	Sets bPickupable to true so that this item can be collected from the world.
	*	11/4/2017 - Matthew Woolley
	*/
	AItem();

	/*
	 *	Called when the player "uses" this item (equipping weapons, taking drugs, etc.).
	 *	11/4/2017 - Matthew Woolley
	 */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnUse();
	virtual void OnUse_Implementation() { return; };

	/*
	*	When two items are combined together, this will find the recipe that is being used to create a new item.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnCombine( AItem* CombinedItem, int32 ItemASlot, int32 ItemBSlot );
	virtual void OnCombine_Implementation( AItem* CombinedItem, int32 ItemASlot, int32 ItemBSlot );

	/*
	 *	Called when the player "examines" the item in the inventory.
	 *	11/4/2017 - Matthew Woolley
	 */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnExamine();
	virtual void OnExamine_Implementation() { return; };

	/*
	 *	Called when the player "interacts" with this item in the world (picking up an item, examining a painting, etc.).
	 *	11/4/2017 - Matthew Woolley
	 */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Item" )
	void OnInteract();
	virtual void OnInteract_Implementation() { return; };

	/*
	*	This attaches the item to the character's mesh's hand so that it follows along with the animations and movement.
	*	11/4/2017 - Matthew Woolley
	*/
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
	bool bPickupable;

	// The crafting recipes for all drugs. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	TArray<FCrafting> CraftingRecipies;

protected:
	// The box that the Character uses to detect this item on the ground. [14/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Item" )
	UBoxComponent* DetectionBox;
};
