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
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ] = ItemBeingAdded;
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ].CurrentItemStack = 1;
				PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ].CurrentItemStack--;
				CreatedDrug->Destroy();
			}

			return;
		}
	}
}
