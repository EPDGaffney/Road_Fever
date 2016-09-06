// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Drug.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



// Called when the user wishes to combine an item. [6/9/2016 Matthew Woolley]
void ADrug::OnCombine_Implementation( AItem* CombinedItem )
{
	for ( FCrafting DrugCraftingRecipe : DrugCraftingRecipes )
	{
		// If the drugs being combined have a crafting recipe. [6/9/2016 Matthew Woolley]
		if ( ( this->IsA( DrugCraftingRecipe.DrugA ) || this->IsA( DrugCraftingRecipe.DrugB ) ) && ( CombinedItem->IsA( DrugCraftingRecipe.DrugA ) || CombinedItem->IsA( DrugCraftingRecipe.DrugB ) ) )
		{
			ADrug* CreatedDrug = Cast<ADrug>( GetWorld()->SpawnActor( DrugCraftingRecipe.CreatedDrug ) );

			FInventoryItem ItemBeingAdded;
			ItemBeingAdded= CreatedDrug->ItemInfo;
			
			ARoadFeverCharacterNed* PlayerCharacter = ( ARoadFeverCharacterNed* ) GetWorld()->GetFirstPlayerController()->GetPawn();

			if ( PlayerCharacter )
			{
				PlayerCharacter->AddItemToInventory( ItemBeingAdded );
				CreatedDrug->Destroy();
			}

			return;
		}
	}
}
