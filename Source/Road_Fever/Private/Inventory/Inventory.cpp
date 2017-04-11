// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Inventory/Inventory.h"
#include "Public/Items/Ammo/Ammo.h"
#include "Characters/RoadFeverCharacterNed.h"



/*
 *	Sets the inventory to default as closed.
 *	Sets up the ticking so that the TickComponent function fires even when paused.
 *	11/4/2017 - Matthew Woolley
 */
UInventory::UInventory()
{
	bOpen = false;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

/*
 *	Ensures that slots with no ammo or stack size is removed from the inventory.
 *	11/4/2017 - Matthew Woolley
 */
void UInventory::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	for ( int32 iItemSlotIterator = 0; iItemSlotIterator < ItemSlots.Num() ; iItemSlotIterator++)
	{
		FInventoryItem SlotItem = ItemSlots[ iItemSlotIterator ];

		// True if there are items in this slot. [10/8/2016 Matthew Woolley]
		bool bHasItems = ( SlotItem.CurrentItemStack > 0 );

		// True if there is ammo in this/these clip(s). [10/8/2016 Matthew Woolley]
		bool bHasAmmo = SlotItem.bClip ? ( ( SlotItem.CurrentAmmo > 0 ) && ( !SlotItem.bWeapon ) ) : bHasItems;

		// If there are no items, or no ammo. [10/8/2016 Matthew Woolley]
		if ( !bHasItems || !bHasAmmo )
		{
			// Empty the slot. [10/8/2016 Matthew Woolley]
			ItemSlots[ iItemSlotIterator ].bClip = false;
			ItemSlots[ iItemSlotIterator ].bEquipable = false;
			ItemSlots[ iItemSlotIterator ].bEquipped = false;
			ItemSlots[ iItemSlotIterator ].bWeapon = false;
			ItemSlots[ iItemSlotIterator ].CurrentAmmo = 0;
			ItemSlots[ iItemSlotIterator ].CurrentItemStack = 0;
			ItemSlots[ iItemSlotIterator ].DisplayIcon = nullptr;
			ItemSlots[ iItemSlotIterator ].DisplayName = "";
			ItemSlots[ iItemSlotIterator ].ItemClass = NULL;
			ItemSlots[ iItemSlotIterator ].ItemToolTip = "";
			ItemSlots[ iItemSlotIterator ].MaxAmmo = 0;
			ItemSlots[ iItemSlotIterator ].MaxItemStack = 0;
		}
	}
}

/*
 *	Toggles the state of the inventory, then calls the function that opens
 *	and closes the UI.
 *	11/4/2017 - Matthew Woolley
 */
void UInventory::ToggleInventory()
{
	ARoadFeverCharacterNed* PlayerCharacter = ( ARoadFeverCharacterNed* ) GetWorld()->GetFirstPlayerController()->GetPawn();

	// If the game should allow input. [29/7/2016 Matthew Woolley]
	if ( PlayerCharacter && !PlayerCharacter->bIsAiming && ( PlayerCharacter->GameHasFocus() || bOpen ) )
	{
		bOpen = !bOpen;
		bOpen ? OpenInv() : CloseInv();
	}
}

/*
 *	Spawns the UI for the player to interact with.
 *	Sets the input mode to focus onto the UI.
 *	Shows the mouse cursor.
 *	11/4/2017 - Matthew Woolley
 */
void UInventory::OpenInv()
{
	// Spawn the InventoryUIWidget if a valid template was provided. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetTemplate )
	{
		UWorld* const World = GetWorld();
		InventoryUIWidgetInstance = CreateWidget<UUserWidget>( World, InventoryUIWidgetTemplate );
	}

	// If the widget was created without any errors. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetInstance && !InventoryUIWidgetInstance->GetIsVisible() )
	{
		// Add it to the view port. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->AddToViewport();

		// Pause the game. [29/7/2016 Matthew Woolley]
		APlayerController* PlayerController = ( APlayerController* ) GEngine->GetFirstLocalPlayerController( GetWorld() );
		PlayerController->SetPause( true );

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus( InventoryUIWidgetInstance->GetCachedWidget() );
		PlayerController->SetInputMode( Mode );
		PlayerController->bShowMouseCursor = true;
	}
}

/*
*	Destroys the UI.
*	Sets the input mode to focus into the game.
*	Hides the mouse cursor.
*	11/4/2017 - Matthew Woolley
*/
void UInventory::CloseInv()
{
	// If there is a widget. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetInstance && InventoryUIWidgetInstance->GetIsVisible() )
	{
		// Remove it from sight. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->RemoveFromParent();

		// Pause the game. [29/7/2016 Matthew Woolley]
		APlayerController* PLayerController = ( APlayerController* ) GEngine->GetFirstLocalPlayerController( GetWorld() );
		PLayerController->SetPause( false );

		FInputModeGameOnly Mode;
		PLayerController->SetInputMode( Mode );
		FSlateApplication::Get().SetFocusToGameViewport();
		PLayerController->bShowMouseCursor = false;

		// Delete the pointer. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance = nullptr;
	}
}

/*
 *	Shows a confirmation screen so that the user can pickup or cancel an item interaction.
 *	Sets the focus of the game onto the UI.
 *	Shows the mouse cursor
 *	11/4/2017 - Matthew Woolley
 */
void UInventory::OpenPickupConfirmation()
{
	// Spawn the InventoryUIWidget if a valid template was provided. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationTemplate )
	{
		UWorld* const World = GetWorld();
		ItemPickupConfirmationInstance = CreateWidget<UUserWidget>( World, ItemPickupConfirmationTemplate );
	}

	// If the widget was created without any errors. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationInstance && !ItemPickupConfirmationInstance->GetIsVisible() )
	{
		// add it to the view port. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance->AddToViewport();

		// Allow the widget to accept input. [22/7/2016 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus( ItemPickupConfirmationInstance->GetCachedWidget() );
		Controller->SetInputMode( Mode );
		Controller->bShowMouseCursor = true;
	}
}

/*
*	Destroys the UI.
*	Sets the input mode to focus into the game.
*	Hides the mouse cursor.
*	11/4/2017 - Matthew Woolley
*/
void UInventory::ClosePickupConfirmation()
{
	// If there is a widget. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationInstance && ItemPickupConfirmationInstance->GetIsVisible() )
	{
		// Remove it from sight. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance->RemoveFromParent();

		// Make the input return to the game. [22/7/2016 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameOnly Mode;
		Controller->SetInputMode( Mode );
		FSlateApplication::Get().SetFocusToGameViewport();
		Controller->bShowMouseCursor = false;

		// Delete the pointer. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance = nullptr;
	}
}
