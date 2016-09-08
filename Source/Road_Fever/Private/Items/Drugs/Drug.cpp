// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Drug.h"
#include "Public/Characters/RoadFeverCharacterNed.h"
#include "Public/Inventory/Inventory.h"



// Called when the user wishes to combine an item. [6/9/2016 Matthew Woolley]
void ADrug::OnCombine_Implementation( AItem* CombinedItem, int32 ItemASlot, int32 ItemBSlot )
{
	if ( CombinedItem == NULL )
		return;

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
				// Make the initial item become the crafted item. [8/9/2016 Matthew Woolley]
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ] = ItemBeingAdded;
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ].CurrentItemStack = 1;

				// Select the initial item's item slot. [8/9/2016 Matthew Woolley]
				PlayerCharacter->CharactersInventory->CurrentActiveSlot = ItemASlot;

				// Remove the item combined with it. [8/9/2016 Matthew Woolley]
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ].CurrentItemStack--;

				// Destroy all the drugs' placeholder objects. [8/9/2016 Matthew Woolley]
				CreatedDrug->Destroy();
				CombinedItem->Destroy();
				this->Destroy();
			}

			return;
		}
	}
}
