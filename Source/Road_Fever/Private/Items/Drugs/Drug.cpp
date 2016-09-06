// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Drug.h"



// Called when the user wishes to combine an item. [6/9/2016 Matthew Woolley]
void ADrug::OnCombine_Implementation( AItem* CombinedItem )
{
	// Get the drugs that are being combined. [6/9/2016 Matthew Woolley]
	TSubclassOf<ADrug> ThisDrug = this->StaticClass();
	TSubclassOf<ADrug> CombinedDrug = CombinedItem->StaticClass();

	for ( FCrafting DrugCraftingRecipe : DrugCraftingRecipes )
	{
		// If the drugs being combined have a crafting recipe. [6/9/2016 Matthew Woolley]
		if ( ( DrugCraftingRecipe.DrugA == ThisDrug || DrugCraftingRecipe.DrugB == ThisDrug ) && ( DrugCraftingRecipe.DrugA == CombinedDrug || DrugCraftingRecipe.DrugB == CombinedDrug ) )
		{
			GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT( "The new drug has been added, you cunt." ) );
		}
	}
}
