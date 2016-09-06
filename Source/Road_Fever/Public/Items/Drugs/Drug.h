// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Items/Item.h"
#include "Drug.generated.h"

// What is required to create a certain drug. [6/9/2016 Matthew Woolley]
USTRUCT( BlueprintType )
struct FCrafting
{
	GENERATED_USTRUCT_BODY()

public:
	// One of the drugs needed. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class ADrug> DrugA;

	// One of the drugs needed. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class ADrug> DrugB;

	// The drug that is created when both DrugA and DrugB are combined. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class ADrug> CreatedDrug;
};

/**
 * Default class for any drug items.
 */
UCLASS()
class ROAD_FEVER_API ADrug : public AItem
{
	GENERATED_BODY()
public:
	// The crafting recipes for all drugs. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TArray<FCrafting> DrugCraftingRecipes;
	
	// Called when the user wishes to combine an item. [6/9/2016 Matthew Woolley]
	virtual void OnCombine_Implementation( AItem* CombinedItem ) override;
};
