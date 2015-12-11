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
	// Pointers for usage [24/11/2015 Matthew Woolley]
	typedef void(*OnUsePointer) (void);
	typedef void(*OnExaminePointer) (void);
	typedef void(*OnCombinePointer) (void);

	// Function pointers for inventory usage [24/11/2015 Matthew Woolley]
	OnUsePointer OnUse;
	OnExaminePointer OnExamine;
	OnCombinePointer OnCombine;

	TSubclassOf<class AItem> ItemClass;

	// The texture that gets shown in the inventory [26/11/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* DisplayIcon;

	// The name that the item has [26/11/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString DisplayName;

	// Called when we want to fill out this struct's details [24/11/2015 Matthew Woolley]
	void SetItem(TSubclassOf<class AItem> NewItemClass, OnUsePointer NewOnUsePointer
		, OnExaminePointer NewOnExaminePointer, OnCombinePointer NewOnCombinePointer
		, UTexture2D* NewDisplayIcon, FString NewDisplayName)
	{
		ItemClass = NewItemClass;
		OnUse = NewOnUsePointer;
		OnCombine = NewOnCombinePointer;
		DisplayIcon = NewDisplayIcon;
		DisplayName = NewDisplayName;
	}
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
};
