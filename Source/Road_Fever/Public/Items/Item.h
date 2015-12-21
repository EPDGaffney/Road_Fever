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
	// Pointers for later usage [24/11/2015 Matthew Woolley]
	typedef void(*OnUsePointer) (void);
	typedef void(*OnExaminePointer) (void);
	typedef void(*OnCombinePointer) (void);

	// Function pointers for inventory usage [24/11/2015 Matthew Woolley]
	OnUsePointer OnUse;
	OnExaminePointer OnExamine;
	OnCombinePointer OnCombine;

	// The class that gets spawned when "Use", or some other variant, has been clicked. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	TSubclassOf<class AItem> ItemClass;

	// The texture that gets shown in the inventory [26/11/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* DisplayIcon;

	// The name that the item has [26/11/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString DisplayName;

	// The text that accompanies this item, once it has been clicked. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	FString ItemToolTip;
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
