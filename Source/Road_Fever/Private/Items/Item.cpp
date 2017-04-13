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

			ARoadFeverCharacterNed* PlayerCharacter = ( ARoadFeverCharacterNed* )GetWorld()->GetFirstPlayerController()->GetPawn();

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
				CreatedItem->Destroy();
				CombinedItem->Destroy();
				this->Destroy();
			}

			return;
		}
	}
}

/*
 *	This attaches the item to the character's mesh's hand so that it follows along with the animations and movement.
 *	11/4/2017 - Matthew Woolley
 */
void AItem::AttachItemToNed_Hand()
{
	ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );
	USkeletalMeshComponent* NedsCharacterMesh = PlayerCharacter->GetMesh();
	bPickupable = false;

	if ( NedsCharacterMesh )
	{
		AttachToComponent( NedsCharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName( TEXT( "ItemPoint" ) ) );
		DetectionBox->DestroyComponent();
	}
}
