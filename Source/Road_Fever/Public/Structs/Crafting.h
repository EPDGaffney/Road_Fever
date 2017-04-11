// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Crafting.generated.h"



/*
 *	The recipe used when any given item is combined with another.
 *	11/4/2017 - Matthew Woolley
 */
USTRUCT( BlueprintType )
struct FCrafting
{
	GENERATED_USTRUCT_BODY()

public:
	// One of the items needed. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class AItem> ItemA;

	// One of the other items needed. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class AItem> ItemB;

	// The drug that is created when both DrugA and DrugB are combined. [6/9/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class AItem> CreatedItem;
};
