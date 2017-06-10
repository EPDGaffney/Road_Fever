// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Item.h"
#include "Public/Characters/RoadFeverCharacterNed.h"
#include "Components/ArrowComponent.h"
#include "Public/Items/Ammo/Ammo.h"
#include "Public/Inventory/Inventory.h"
#include "Public/Items/Weapons/Weapon.h"



/*
 *	Creates a box that is used by the character so that the character can detect the item.
 *	Creates a StaticMeshComponent that stores the mesh of the item.
 *	Sets bPickupable to true so that this item can be collected from the world.
 *	11/4/2017 - Matthew Woolley
 */
AItem::AItem()
{
	// Create a generic scene object to give the item world transform. [13/4/2017 Matthew Woolley]
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );
	RootComponent = Root;

	// Create the default detection box. [14/7/2016 Matthew Woolley]
	DetectionBox = CreateDefaultSubobject<UBoxComponent>( TEXT( "Detection Box" ) );
	DetectionBox->SetBoxExtent( FVector( 32, 32, 32 ) );
	DetectionBox->RelativeLocation = FVector( 0, 0, 32 );
	DetectionBox->SetupAttachment( RootComponent );
	DetectionBox->SetCollisionProfileName( "ItemPickup" );

	// Setup this Item's mesh. [14/7/2016 Matthew Woolley]
	ItemsMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Item's Mesh" ) );
	ItemsMesh->SetupAttachment( RootComponent );
	ItemsMesh->SetCollisionProfileName( "NoCollision" );

	// Setup some generic properties. [20/7/2016 Matthew Woolley]
	bPickupable = true;
}

/*
 *	When two items are combined together, this will find the recipe that is being used to create a new item.
 *	11/4/2017 - Matthew Woolley
 */
void AItem::OnCombine_Implementation( AItem* CombinedItem, int32 ItemASlot, int32 ItemBSlot )
{
	if ( CombinedItem == NULL )
		return;

	for ( FCrafting CraftingRecipe : CraftingRecipies )
	{
		// If the drugs being combined have a crafting recipe. [6/9/2016 Matthew Woolley]
		if ( ( this->IsA( CraftingRecipe.ItemA ) || this->IsA( CraftingRecipe.ItemB ) ) && ( CombinedItem->IsA( CraftingRecipe.ItemA ) || CombinedItem->IsA( CraftingRecipe.ItemB ) ) )
		{
			AItem* CreatedItem = Cast<AItem>( GetWorld()->SpawnActor( CraftingRecipe.CreatedItem ) );

			FInventoryItem ItemBeingAdded;
			ItemBeingAdded = CreatedItem->ItemInfo;

			ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

			if ( CreatedItem && PlayerCharacter )
			{
				// If this is a clip that is receiving ammo. [6/6/2017 Matthew Woolley]
				if ( CreatedItem->ItemInfo.bClip )
				{
					// If the CombinedItem is a clip. [6/6/2017 Matthew Woolley]
					if ( CombinedItem->ItemInfo.bClip )
					{
						FInventoryItem ItemThatIsClip = CombinedItem->ItemInfo.bClip ? PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ] : ItemInfo;
						FInventoryItem ItemThatIsAmmoStash = ItemInfo.bClip ? PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ] : ItemInfo;

						// If there is more ammo than space in the clip. [6/6/2017 Matthew Woolley]
						if ( ( ItemThatIsClip.CurrentAmmo + ItemThatIsAmmoStash.CurrentAmmo ) > ItemThatIsClip.MaxAmmo )
						{
							// Get the amount of ammo that is currently needed from the ammo stash, and remove it. [6/6/2017 Matthew Woolley]
							int AmountToRemoveFromBulletStash = CreatedItem->ItemInfo.MaxAmmo - ItemThatIsClip.CurrentAmmo;
							PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ].CurrentAmmo -= AmountToRemoveFromBulletStash;

							// Fill the clip. [6/6/2017 Matthew Woolley]
							PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ].CurrentAmmo = CreatedItem->ItemInfo.MaxAmmo;
						}
						else
						{
							// Fill the clip. [6/6/2017 Matthew Woolley]
							PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ].CurrentAmmo += ItemThatIsAmmoStash.CurrentAmmo;

							// Remove the ammo from the stash. [6/6/2017 Matthew Woolley]
							PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ].CurrentAmmo = 0;
						}
					}
				}
				else
				{
					// Remove items from the inventory that were used to craft the new item. [14/4/2017 Matthew Woolley]
					PlayerCharacter->CharactersInventory->ItemSlots[ ItemASlot ].CurrentItemStack--;
					PlayerCharacter->CharactersInventory->ItemSlots[ ItemBSlot ].CurrentItemStack--;
					
					// Add the item that was crafted to the inventory. [14/4/2017 Matthew Woolley]
					PlayerCharacter->AddItemToInventory( CreatedItem->ItemInfo );
				}

				// Select the initial item's item slot. [8/9/2016 Matthew Woolley]
				PlayerCharacter->CharactersInventory->CurrentActiveSlot = ItemASlot;

				// Destroy all the drugs' placeholder objects. [8/9/2016 Matthew Woolley]
				CreatedItem->Destroy();
				CombinedItem->Destroy();
				this->Destroy();
			}
			
			// Destroy all the drugs' placeholder objects. [8/9/2016 Matthew Woolley]
			CreatedItem->Destroy();
			CombinedItem->Destroy();
			this->Destroy();

			return;
		}
	}

	CombinedItem->Destroy();
	this->Destroy();
}

/*
 *	This attaches the item to the character's mesh's hand so that it follows along with the animations and movement.
 *	11/4/2017 - Matthew Woolley
 */
void AItem::AttachItemToNed_Hand()
{
	ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

	if ( PlayerCharacter )
	{
		USkeletalMeshComponent* NedsCharacterMesh = PlayerCharacter->GetMesh();

		if ( NedsCharacterMesh )
		{
			AttachToComponent( NedsCharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName( TEXT( "ItemPoint" ) ) );
			bPickupable = false;
			DetectionBox->DestroyComponent();
		}
	}
}
